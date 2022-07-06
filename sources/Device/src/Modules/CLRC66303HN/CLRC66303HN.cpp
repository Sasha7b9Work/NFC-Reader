// 2022/7/3 11:16:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"


namespace CLRC66303HN
{
    bool DetectCard();

    void WriteRegister(uint8 reg, uint8 data);
    uint8 ReadRegister(uint8 reg);
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


void CLRC66303HN::WriteRegister(uint8 reg, uint8 data)
{
    uint8 buffer[2] = { (uint8)(reg << 1), data };

    HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
}


uint8 CLRC66303HN::ReadRegister(uint8 reg)
{
    uint8 out[2] = { (uint8)((reg << 1) | 1), 0 };
    uint8 in[2];

    HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 2);

    return in[1];
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
    6:  writeRegister(0x28, 0x8E);
    7:  writeRegister(0x06, 0x7F);
    8:  writeRegister(0x2C, 0x18);
    9:  writeRegister(0x2D, 0x18);
    10: writeRegister(0x2E, 0x0F);
    11: writeRegister(0x05, 0x26);
    12: writeRegister(0x00, 0x07);
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

    Register::DrvMode().Write(true, false, false, 0x00);

    return false;
}
