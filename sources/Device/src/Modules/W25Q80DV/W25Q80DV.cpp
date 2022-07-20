// 2022/6/10 9:08:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Utils/Buffer.h"


/*
*   Block 0
*   Sector 0
*   Адреса 000000h - 0000FFh
*/

//                            page
#define PROGRAM_PAGE  0x02  /* 34 */
#define READ_DATA     0x03  /* 26 */
#define WRITE_DISABLE 0x04  /* 23 */
#define READ_STATUS_1 0x05  /* 24 */
#define WRITE_ENABLE  0x06  /* 22 */
#define SECTOR_ERASE  0x20  /* 36 */


namespace W25Q80DV
{
    // Записывает uin8, а затем младшие 3 байта из второго значения
    void Write32bit(uint8, uint);

    bool IsBusy();

    void WaitRelease();

    uint8 test_value = 0;
}


void W25Q80DV::Write1024bytes(const uint8 *buffer, int size)
{
    WaitRelease();

    HAL_SPI::Write(DirectionSPI::Memory, WRITE_ENABLE);               // Write enable

    Write32bit(SECTOR_ERASE, 0x000000);         // Sector erase

    HAL_SPI::Write(DirectionSPI::Memory, WRITE_DISABLE);              // Write disable

    WaitRelease();

    HAL_SPI::Write(DirectionSPI::Memory, WRITE_ENABLE);               // Write enable

    Buffer<uint8, 1024> data;

    data[0] = PROGRAM_PAGE;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    for (int i = 0; i < size; i++)
    {
        data[4 + i] = buffer[i];
    }

    //                                команда адрес
    HAL_SPI::Write(DirectionSPI::Memory, data.Data(), size +   1   +  3);     // Page program

    HAL_SPI::Write(DirectionSPI::Memory, WRITE_DISABLE);              // Write disable
}


void W25Q80DV::Read1024bytes(uint8 *buffer, int size)
{
    WaitRelease();

    Buffer<uint8, 1024> out;

    out[0] = READ_DATA;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;

    Buffer<uint8, 1024> in;

    HAL_SPI::WriteRead(DirectionSPI::Memory, out.Data(), in.Data(), size + 1 + 3);

    for (int i = 0; i < size; i++)
    {
        buffer[i] = in[4 + i];
    }
}


void W25Q80DV::Write32bit(uint8 command, uint bits24)
{
    uint8 data[4];

    data[0] = command;
    data[1] = (uint8)(bits24 >> 16);
    data[2] = (uint8)(bits24 >> 8);
    data[3] = (uint8)(bits24);

    HAL_SPI::Write(DirectionSPI::Memory, data, 4);
}


bool W25Q80DV::IsBusy()
{
    static const uint8 out[2] = { READ_STATUS_1, 0 };
    static uint8 in[2] = { 0, 0 };

    HAL_SPI::WriteRead(DirectionSPI::Memory, out, in, 2);

    return (in[1] & 0x01);
}


void W25Q80DV::WaitRelease()
{
    TimeMeterMS meter;

    while (IsBusy() && (meter.ElapsedTime() < 1000))
    {
    }
}


void W25Q80DV::ReadID()
{
    uint8 out[6] = { 0x90, 0, 0, 0, 0, 0 };
    uint8 in[6] = { 0, 0, 0, 0, 0, 0 };

    HAL_SPI::WriteRead(DirectionSPI::Memory, out, in, 6);
}


bool W25Q80DV::Test()
{
    uint8 out[256] = { 0x55 };
    uint8 in[256] = { 0x00 };

    Write1024bytes(out, 1);

    Read1024bytes(in, 1);

    test_value = in[0];

    return (std::memcmp(out, in, 1) == 0);
}


uint8 W25Q80DV::TestValue()
{
    return test_value;
}
