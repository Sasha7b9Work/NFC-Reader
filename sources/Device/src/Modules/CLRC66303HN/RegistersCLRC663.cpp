// 2022/7/6 9:44:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"


void Register::RegisterCLRC663::Write()
{
    uint8 buffer[2] = { address, (uint8)data };

    HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
}


void Register::FIFOControl::Write(Size::E size, bool clear, int waterLevelExtBit)
{

}


void Register::FIFOData::Write(uint8 data0, uint8 data1)
{

}


void Register::DrvMode::Write(bool Tx2Inv, bool Tx1Inv, bool TxEn, uint8 TxClkMode)
{

}
