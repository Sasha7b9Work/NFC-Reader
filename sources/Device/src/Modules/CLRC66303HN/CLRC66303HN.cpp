// 2022/7/3 11:16:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>
#include <cstdio>


namespace CLRC66303HN
{
    uint8 ReadRegister(uint8);

    void WriteRegister(uint8 reg, uint8 value);

    // Подача питания на чип
    namespace Power
    {
        void Off()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
        }

        void On()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        }

        void Init()
        {
            GPIO_InitTypeDef is =
            {
                GPIO_PIN_6,                     // ENN
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH
            };

            HAL_GPIO_Init(GPIOA, &is);
        }
    }

    // Включение электромагнитного поля
    namespace RF
    {
        void On()
        {
            uint8 reg = ReadRegister(0x28);
            _SET_BIT(reg, 3);
            WriteRegister(0x28, reg);
        }

        void Off()
        {
            uint8 reg = ReadRegister(0x28);
            _CLEAR_BIT(reg, 3);
            WriteRegister(0x28, reg);
        }
    }


    bool DetectCard();

    static UID uid;

    static bool detected = false;
}


void CLRC66303HN::Init()
{
    Power::Init();

    Power::On();

    Timer::Delay(100);

    RF::Off();

    HAL_FLASH::LoadAntennaConfiguration106();

    HAL_FLASH::LoadProtocol();

    fifo.Init();

    WriteRegister(0x09, 0xC0);      // IRQ1En           // Включаем пин прерывание в Push-Pull

    while ((irq0.GetValue() & IRQ0::IdleIRQ) == 0)
    {
    }

    Timer::Delay(10);

    WriteRegister(0x08, 0x84);      // IRQ0En           // Включаем "железное прерываниие" IRQ на чтение данных. Инвертируем

    while ((irq0.GetValue() & IRQ0::IdleIRQ) == 0)
    {
    }

    Timer::Delay(10);
}


void CLRC66303HN::Update()
{
    uid.Clear();

    if (DetectCard())
    {
        if (!detected)
        {
            HAL_USART2_WG26::TransmitUID(uid);

            /*
            if (HAL_USART2_WG26::InModeUART())
            {
                HAL_USART2_WG26::Transmit("%s\x0D\x0A", uid.ToString());
            }
            else
            {
                HAL_USART2_WG26::TransmitRAW(uid.uid[0], uid.ui)
            }
            */
        }

        detected = true;
    }
    else
    {
        detected = false;
    }
}


CLRC66303HN::UID &CLRC66303HN::GetUID()
{
    return uid;
}


bool CLRC66303HN::DetectCard()
{
    Command::Idle();

    fifo.Clear();
    RF::On();

    TimeMeterUS meter;

    while (meter.ElapsedUS() < 5100)
    {
    }

    irq0.Clear();                   // IRQ0

    Register::RegisterCLRC663(0x2C).Write(0x18);        // Switches the CRC extention OFF in tx direction
    Register::RegisterCLRC663(0x2D).Write(0x18);        // Switches the CRC extention OFF in rx direction

    Register::RegisterCLRC663(0x2E).Write(0x0F);        // Only the 7 last bits will be sent via NFC

    Command::Card::Send(0x26);                          // REQA

    BitSet16 data;

    while (meter.ElapsedUS() < 8000)                    // Запрос REQA
    {
        if (irq0.DataReady())                           // данные получены
        {
            uint8 reg_0x06 = irq0.GetValue();

            if (reg_0x06 & IRQ0::ErrIRQ)                // ошибка данных
            {
                break;
            }
            else                                        // данные верны
            {
                data.byte[0] = fifo.Pop();
                data.byte[1] = fifo.Pop();

                break;
            }
        }
    }

    if (data.half_word != 0)
    {
        if (Command::Card::AnticollisionCL(1, &uid))
        {
            if (Command::Card::SelectCL(1, &uid))
            {
                if (!uid.Calcualted())
                {
                    if (Command::Card::AnticollisionCL(2, &uid))
                    {
                        Command::Card::SelectCL(2, &uid);
                    }
                }
            }
        }
    }

    RF::Off();

    return uid.Calcualted();
}


uint8 CLRC66303HN::ReadRegister(uint8 reg)
{
    uint8 out[2] = { (uint8)((reg << 1) | 1), 0 };
    uint8 in[2];

    HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 2);

    return in[1];
}


void CLRC66303HN::WriteRegister(uint8 reg, uint8 value)
{
    uint8 out[2] = { (uint8)(reg << 1), value };

    HAL_SPI::Write(DirectionSPI::Reader, out, 2);
}
