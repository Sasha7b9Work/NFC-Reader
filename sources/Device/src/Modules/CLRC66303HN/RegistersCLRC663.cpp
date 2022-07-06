// 2022/7/6 9:44:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


void Register::RegisterCLRC663::Write()
{
    uint8 buffer[2] = { (uint8)(address << 1), (uint8)data };

    HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
}


void Register::RegisterCLRC663::Write(uint8 _data)
{
    data = _data;

    Write();
}


uint8 Register::RegisterCLRC663::Read()
{
    uint8 out[2] = { (uint8)((address << 1) | 1), 0 };
    uint8 in[2];

    HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 2);

    data = in[1];

    return (uint8)data;
}


void Register::FIFOControl::Write(Size::E /*size*/, bool /*clear*/, int /*waterLevelExtBit*/)
{

}


void Register::FIFOData::Write(uint8 /*data*/)
{

}


void Register::FIFOData::Write(uint8 /*data0*/, uint8 /*data1*/)
{

}


void Register::FIFOData::Read2Bytes(uint8 _data[2])
{
    uint8 out[3] = { (uint8)address, 0x05, 0x00 };
    uint8 in[3];

    HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 3);

    std::memcpy(_data, &in[1], 2);
}
