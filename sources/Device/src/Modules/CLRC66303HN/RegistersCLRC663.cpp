// 2022/7/6 9:44:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>
#include <cstring>


namespace CLRC66303HN
{
    FIFO fifo;

    IRQ0 irq0;

    uint8 ReadRegister(uint8);

    void WriteRegister(uint8 reg, uint8 value);


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


    void FIFO::Init()
    {
        WriteRegister(0x02, 0xB0);
    }


    void FIFO::Clear()
    {
        WriteRegister(0x02, 0xB0);
    }


    void FIFO::Push(uint8 data)
    {
        WriteRegister(0x05, data);
    }


    uint8 FIFO::Pop()
    {
        return ReadRegister(0x05);
    }


    void IRQ0::Clear()
    {
        WriteRegister(0x06, 0x7F);      // Clears all bits in IRQ0
    }


    uint8 IRQ0::GetValue()
    {
        return ReadRegister(0x06);
    }


    bool IRQ0::DataReadyHardware()
    {
        return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET;
    }
}
