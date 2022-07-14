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

    bool DetectCard1();

    bool DetectCard();

    static void LoadAntennaConfiguration106();

    static void LoadProtocol();

    static bool detected = false;       // true, если карта детектирована

    static UID uid;

    BitSet16 data;
}


void CLRC66303HN::Init()
{
    Power::Init();

    Power::On();

    Timer::Delay(100);

    RF::Off();

    LoadAntennaConfiguration106();
}


void CLRC66303HN::Update()
{
    gf.Clear();

    if (DetectCard())
    {
        if (!detected)
        {
            HAL_USART2::TransmitRAW("Card detected.");
        }

        detected = true;
    }
    else
    {
        if (detected)
        {
            HAL_USART2::TransmitRAW("Card lost.");
        }

        detected = false;
    }
}


CLRC66303HN::UID &CLRC66303HN::GetUID()
{
    return uid;
}


bool CLRC66303HN::DetectCard()
{
    bool result = false;

    TimeMeterUS meter;

    Register::RegisterCLRC663(0x00).Write(0x00);        // Cancels previous executions and the state machine returns into IDLE mode
    Register::RegisterCLRC663(0x02).Write(0xB0);        // Flushes the FIFO and defines FIFO characteristics

    LoadProtocol();

    Register::RegisterCLRC663(0x02).Write(0xB0);        // Flushes the FIFO and defines FIFO characteristics
    RF::On();                                           // Switches the RF filed ON.

    while (meter.ElapsedUS() < 5100)
    {
    }

    Register::RegisterCLRC663(0x06).Write(0x7F);        // Clears all bits in IRQ0

    Register::RegisterCLRC663(0x2C).Write(0x18);        // Switches the CRC extention OFF in tx direction
    Register::RegisterCLRC663(0x2D).Write(0x18);        // Switches the CRC extention OFF in rx direction

    Register::RegisterCLRC663(0x2E).Write(0x0F);        // Only the 7 last bits will be sent via NFC
    Register::RegisterCLRC663(0x05).Write(0x26);        // Fills the FIFO with 0x26 (REQA)
    Register::RegisterCLRC663(0x00).Write(0x07);        // Executes Transceive routine

    while (meter.ElapsedUS() < 7000)                                                        // Запрос REQA
    {
        uint8 reg_0x06 = Register::RegisterCLRC663(0x06).Read();

        if (reg_0x06 & Register::IRQ0::RxIRQ)                       // данные получены
        {
            if (reg_0x06 & Register::IRQ0::ErrIRQ)                  // ошибка данных
            {
                data.byte[0] = Register::RegisterCLRC663(0x05).Read();
                data.byte[1] = Register::RegisterCLRC663(0x05).Read();

                data.half_word = (uint16)(-1);

                Register::RegisterCLRC663(0x05).Write(0x26);        // Fills the FIFO with 0x26 (REQA)
                Register::RegisterCLRC663(0x00).Write(0x07);        // Executes Transceive routine
            }
            else                                                    // данные верны
            {
                result = true;
                gf.num_result++;

                data.byte[0] = Register::RegisterCLRC663(0x05).Read();
                data.byte[1] = Register::RegisterCLRC663(0x05).Read();

                break;
            }
        }
    }

    uid.Clear();

    if (result)
    {
        gf.num_result++;
        result = Request::AnticollisionCL1().Transceive(&uid);
    }

    if (result)
    {
        gf.num_result++;
        result = Request::SelectCL1().Transceive(&uid);
    }

    if (!uid.calculated)
    {
        if (result)
        {
            gf.num_result++;
            result = Request::AnticollisionCL2().Transceive(&uid);
        }

        if (result)
        {
            gf.num_result++;
            result = Request::SelectCL2().Transceive(&uid);
        }
    }

    /*
    if (result)                                                                         // Anticollision CL1
    {
        meter.Reset();

        Register::RegisterCLRC663(0x00).Write(0x00);        // Cancels previous executions and the state machine returns into IDLE mode
        Register::RegisterCLRC663(0x02).Write(0xB0);        // Flushes the FIFO and defines FIFO characteristics

        Register::RegisterCLRC663(0x06).Write(0x7F);        // Clears all bits in IRQ0
        Register::RegisterCLRC663(0x2E).Write(0x08);        // All bits will be sent via NFC

        Register::RegisterCLRC663(0x05).Write(0x93);        // CL1  \ Anticollision 
        Register::RegisterCLRC663(0x05).Write(0x20);        //      / CL1
        Register::RegisterCLRC663(0x00).Write(0x07);        // Transceive routine

        while (meter.ElapsedUS() < 10000)
        {
            reg_0x06 = Register::RegisterCLRC663(0x06).Read();

            if (reg_0x06 & Register::IRQ0::RxIRQ)
            {
                if (reg_0x06 & Register::IRQ0::ErrIRQ)
                {
                    readed = "error";

                    result = false;

                    break;
                }
                else
                {
                    uid.byte0 = Register::RegisterCLRC663(0x05).Read();         // Читаем первый уровень UID
                    uid.byte1 = Register::RegisterCLRC663(0x05).Read();
                    uid.byte2 = Register::RegisterCLRC663(0x05).Read();
                    uid.byte3 = Register::RegisterCLRC663(0x05).Read();
                    uid.byte4 = Register::RegisterCLRC663(0x05).Read();

                    readed = "yes";

                    break;
                }
            }
        }
    }
    */

    RF::Off();

    return result;
}


static void CLRC66303HN::LoadAntennaConfiguration106()
{
    RF::Off();                                      // DrvMode
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


bool CLRC66303HN::DetectCard1()
{
    /*
            AN12657.pdf

    1:  writeRegister(0x00, 0x00);          command idle
    2:  writeRegister(0x02, 0xB0);          FIFOControl
    3:  writeRegister(0x05, 0x00, 0x00);    FIFOData
    4:  writeRegister(0x00, 0x0D);          command LoadProtocol
    5:  writeRegister(0x02, 0xB0);          FIFOControl
    6:  writeRegister(0x28, 0x8E);          DrvMode
    7:  writeRegister(0x06, 0x7F);          IRQ0
    8:  writeRegister(0x2C, 0x18);          TxCrcPreset
    9:  writeRegister(0x2D, 0x18);          RxCrcCon
    10: writeRegister(0x2E, 0x0F);          TxDataNum
    11: writeRegister(0x05, 0x26);          FIFOData
    12: writeRegister(0x00, 0x07);          Transceive
    13: waitForCardResponse();              IRQ0
    14: readRegister(0x05, 0x05, 0x00);     FIFOData

    */

    TimeMeterUS meter;

    Command::Idle().Run();                                                          // 1

    Register::FIFOControl().Write(Register::FIFOControl::Size::_255, true, 0);      // 5

    while (meter.ElapsedUS() < 1000)
    {
    }

    Register::IRQ0 reg_irq0;                                                        // 7 Очистка битов irq0
    reg_irq0.Write(0x7F);

    Command::Transceive().Run(0x26);    // REQA                                     // 11, 12  Запрос на карту

    while (meter.ElapsedUS() < 6000)
    {
        if (reg_irq0.Read() & Register::IRQ0::RxSOFIRQ)                             // Обнаружена SOF или поднесушая
        {
            return true;
        }
    }

    return false;
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
