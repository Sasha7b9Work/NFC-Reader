// 2022/7/6 9:44:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


namespace CLRC66303HN
{

    void Register::RegisterCLRC663::Write()
    {
        uint8 buffer[2] = { (uint8)(address << 1), (uint8)data };

        HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
    }


    void Register::RegisterCLRC663::Write(uint8 data1, uint8 data2)
    {
        uint8 buffer[3] = { (uint8)(address << 1), data1, data2 };

        HAL_SPI::Write(DirectionSPI::Reader, buffer, 3);
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


    void Register::RegisterCLRC663::Read(uint8 _out[2], uint8 _in[2])
    {
        uint8 out[3] = { (uint8)((address << 1) | 1), _out[0], _out[1] };
        uint8 in[3];

        HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 3);

        _in[0] = in[1];
        _in[1] = in[2];
    }


    void Register::FIFOControl::Write(Size::E size, bool clear, int waterLevelExtBit)
    {
        data = 0;

        if (size == Size::_255)
        {
            _SET_BIT(data, 7);
        }

        if (clear)
        {
            _SET_BIT(data, 4);
        }

        if (waterLevelExtBit)
        {
            _SET_BIT(data, 2);
        }

        RegisterCLRC663::Write();
    }


    void Register::FIFOControl::Clear256()
    {
        Register::RegisterCLRC663(0x02).Write(0xB0);
    }


    void Register::FIFOData::Write(uint8 _data)
    {
        data = _data;

        RegisterCLRC663::Write();
    }


    void Register::FIFOData::Write(uint8 data0, uint8 data1)
    {
        uint8 buffer[3] = { (uint8)(address << 1), data0, data1 };

        HAL_SPI::Write(DirectionSPI::Reader, buffer, 3);

        data = data1;
    }


    void Register::FIFOData::Read2Bytes(uint8 _data[2])
    {
        uint8 out[3] = { (uint8)((address << 1) | 1), 0x05, 0x00 };
        uint8 in[3];

        HAL_SPI::WriteRead(DirectionSPI::Reader, out, in, 3);

        std::memcpy(_data, &in[1], 2);
    }


    void Register::IRQ0::Clear()
    {
        Register::RegisterCLRC663(0x06).Write(0x7F);
    }
}
