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

            Off();
        }
    }

    bool DetectCard();
}


void CLRC66303HN::Init()
{
    Power::Init();
}


void CLRC66303HN::Update()
{
    Power::On();

    if (DetectCard())
    {
        HAL_USART2::TransmitRAW("Card detected");
    }

    Power::Off();
}


bool CLRC66303HN::DetectCard()
{
    /*
    * 
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

    Register::IRQ0 reg_irq0;

    Command::Idle().Run();                                                          // 1

    Register::FIFOControl().Write(Register::FIFOControl::Size::_256, true, 0);      // 2

    Command::LoadProtocol().Run(0x00, 0x00);                                        // 3, 4

    Register::FIFOControl().Write(Register::FIFOControl::Size::_256, true, 0);      // 5

    Register::DrvMode().Write(true, false, true, 0x06);                             // 6

    reg_irq0.Write(0x7F);                                                           // 7

    Register::TxCrcPreset(0x18).Write();                                            // 8

    Register::RxCrcCon(0x18).Write();                                               // 9

    Register::TxDataNum(0x0F).Write();                                              // 10

    uint8 value_irq0 = reg_irq0.Read();

    Command::Transceive().Run(0x26);    // REQA                                     // 11, 12

    TimeMeterMS meter;

    while ((reg_irq0.Read() == value_irq0) && meter.ElapsedTime() < 6)              // 13
    {
    }

    HAL_USART2::Transmit("irq1: before:%02Xh, after:%02Xh", value_irq0, reg_irq0.Read());

    uint8 fifo_data[2] = { 0 , 0 };

    Register::FIFOData().Read2Bytes(fifo_data);                                     // 14

    HAL_USART2::Transmit("fifo: %02Xh %02Xh", fifo_data[0], fifo_data[1]);

    return (value_irq0 != reg_irq0.Read());
}
