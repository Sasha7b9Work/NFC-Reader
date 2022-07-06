// 2022/7/3 11:16:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"


namespace CLRC66303HN
{
    bool DetectCard();
}


void CLRC66303HN::Init()
{

}


void CLRC66303HN::Update()
{
    if (DetectCard())
    {
        HAL_USART2::Transmit("Card detected");
    }
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
    13: waitForCardResponse();
    14: readRegister(0x05, 0x05, 0x00);

    */

    // 1

    Command::Idle().Run();

    // 2

    Register::FIFOControl().Write(Register::FIFOControl::Size::_256, true, 0);

    // 3, 4

    Command::LoadProtocol().Run(0x00, 0x00);

    // 5

    Register::FIFOControl().Write(Register::FIFOControl::Size::_256, true, 0);

    // 6

    Register::DrvMode().Write(true, false, true, 0x06);

    // 7

    Register::IRQ0(0x7F).Write();

    // 8

    Register::TxCrcPreset(0x18).Write();

    // 9

    Register::RxCrcCon(0x18).Write();

    // 10

    Register::TxDataNum(0x0F).Write();

    uint8 irq1 = Register::IRQ0().Read();

    // 11, 12

    Command::Transceive().Run(0x26);

    // 13

    while (Register::IRQ0().Read() == irq1)
    {
    }

    return false;
}
