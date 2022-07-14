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


    void DetectCard();

    static void LoadAntennaConfiguration106();

    static void LoadProtocol();

    static UID uid;
}


void CLRC66303HN::Init()
{
    Power::Init();

    Power::On();

    Timer::Delay(100);

    RF::Off();

    LoadAntennaConfiguration106();

    fifo.Init();
}


void CLRC66303HN::Update()
{
    gf.Clear();

    DetectCard();
}


CLRC66303HN::UID &CLRC66303HN::GetUID()
{
    return uid;
}


void CLRC66303HN::DetectCard()
{
    Command::Idle();

    LoadProtocol();

    fifo.Clear();
    RF::On();

    TimeMeterUS meter;

    while (meter.ElapsedUS() < 5100)
    {
    }

    irq0.Clear();

    Register::RegisterCLRC663(0x2C).Write(0x18);        // Switches the CRC extention OFF in tx direction
    Register::RegisterCLRC663(0x2D).Write(0x18);        // Switches the CRC extention OFF in rx direction

    Register::RegisterCLRC663(0x2E).Write(0x0F);        // Only the 7 last bits will be sent via NFC

    Command::Card::Send(0x26);                          // REQA

    BitSet16 data;

    while (meter.ElapsedUS() < 8000)                    // Запрос REQA
    {
        uint8 reg_0x06 = irq0.GetValue();

        if (reg_0x06 & IRQ0::RxIRQ)                     // данные получены
        {
            if (reg_0x06 & IRQ0::ErrIRQ)                // ошибка данных
            {
                break;
            }
            else                                        // данные верны
            {
                gf.num_result++;

                data.byte[0] = fifo.Pop();
                data.byte[1] = fifo.Pop();

                break;
            }
        }
    }

    if (data.half_word == 0)
    {
        return;
    }

    uid.Clear();

    if (data.half_word != 0)
    {
        gf.num_result++;

        if (Command::Card::AnticollisionCL1().Transceive(&uid))
        {
            gf.num_result++;

            if (Command::Card::SelectCL1().Transceive(&uid))
            {
                gf.num_result++;

                if (!uid.calculated)
                {
                    gf.num_result++;

                    if (Command::Card::AnticollisionCL2().Transceive(&uid))
                    {
                        gf.num_result++;

                        if (Command::Card::SelectCL2().Transceive(&uid))
                        {
                            gf.num_result++;
                        }
                    }
                }
            }
        }
    }

    RF::Off();
}


static void CLRC66303HN::LoadAntennaConfiguration106()
{
    Register::RegisterCLRC663(0x28).Write(0x86);    // DrvMode
    Register::RegisterCLRC663(0x29).Write(0x1F);    // TxAmp
    Register::RegisterCLRC663(0x2A).Write(0x39);    // DrvCon
    Register::RegisterCLRC663(0x2B).Write(0x0A);    // Txl
    Register::RegisterCLRC663(0x2C).Write(0x18);    // TXCrcPreset
    Register::RegisterCLRC663(0x2D).Write(0x18);    // RxCrcCon
    Register::RegisterCLRC663(0x2E).Write(0x0F);    // TxDataNum
    Register::RegisterCLRC663(0x2F).Write(0x21);    // TxModWidth
    Register::RegisterCLRC663(0x30).Write(0x00);    // TxSym10BurstLen
    Register::RegisterCLRC663(0x31).Write(0xC0);    // TxWaitCtrl
    Register::RegisterCLRC663(0x32).Write(0x12);    // TxWaitLo
    Register::RegisterCLRC663(0x33).Write(0xCF);    // TxFrameCon
    Register::RegisterCLRC663(0x34).Write(0x00);    // RsSofD
    Register::RegisterCLRC663(0x35).Write(0x04);    // RxCtrl
    Register::RegisterCLRC663(0x36).Write(0x90);    // RxWait
    Register::RegisterCLRC663(0x37).Write(0x5C);    // RxThreshold
    Register::RegisterCLRC663(0x38).Write(0x12);    // Rcv
    Register::RegisterCLRC663(0x39).Write(0x0A);    // RxAna
}


static void CLRC66303HN::LoadProtocol()
{
    Register::RegisterCLRC663(0x48).Write(0x20);    // TxBitMod     20
    Register::RegisterCLRC663(0x49).Write(0x00);    // RFU          00
    Register::RegisterCLRC663(0x4A).Write(0x04);    // TxDataCon    04
    Register::RegisterCLRC663(0x4B).Write(0x50);    // TxDataMod    50
    Register::RegisterCLRC663(0x4C).Write(0x40);    // TxSymFreq    40
    Register::RegisterCLRC663(0x4D).Write(0x00);    // TxSym0H      00
    Register::RegisterCLRC663(0x4E).Write(0x00);    // TxSym0L      00
    Register::RegisterCLRC663(0x4F).Write(0x00);    // TxSym1H      00
    Register::RegisterCLRC663(0x50).Write(0x00);    // TxSym1L      00
    Register::RegisterCLRC663(0x51).Write(0x00);    // TxSym2       00
    Register::RegisterCLRC663(0x52).Write(0x00);    // TxSym3       00
    Register::RegisterCLRC663(0x53).Write(0x00);    // TxSym10Len   00
    Register::RegisterCLRC663(0x54).Write(0x00);    // TxSym32Len   00
    Register::RegisterCLRC663(0x55).Write(0x00);    // TxSym10BurstCtrl 00
    Register::RegisterCLRC663(0x56).Write(0x00);    // TxSym10Mod   00
    Register::RegisterCLRC663(0x57).Write(0x50);    // TxSym32Mod   50
    Register::RegisterCLRC663(0x58).Write(0x02);    // RxBitMod     02
    Register::RegisterCLRC663(0x59).Write(0x00);    // RxEofSym     00
    Register::RegisterCLRC663(0x5A).Write(0x00);    // RxSyncValH   00
    Register::RegisterCLRC663(0x5B).Write(0x01);    // RxSyncVaIL   01
    Register::RegisterCLRC663(0x5C).Write(0x00);    // RxSyncMod    00
    Register::RegisterCLRC663(0x5D).Write(0x08);    // RxMod        08
    Register::RegisterCLRC663(0x5E).Write(0x80);    // RxCorr       80
    Register::RegisterCLRC663(0x5F).Write(0xB2);    // FabCal       B2
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
