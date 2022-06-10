// 2022/6/10 9:08:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Hardware/HAL/HAL.h"


/*
*   Block 0
*   Sector 0
*   Адреса 000000h - 0000FFh
*/

//                            page
#define WRITE_ENABLE  0x06  /* 22 */
#define SECTOR_ERASE  0x20  /* 36 */
#define WRITE_DISABLE 0x04  /* 23 */
#define PROGRAM_PAGE  0x02  /* 34 */
#define READ_STATUS_1 0x05  /* 24 */


namespace W25Q80DV
{
    void Write24bit(uint8, uint);

    bool IsBusy();

    void WaitRelease();
}


void W25Q80DV::Write256bytes(uint8 *buffer)
{
    HAL_SPI::Init();

    HAL_SPI::Write(WRITE_ENABLE);

    Write24bit(SECTOR_ERASE, 0);

    HAL_SPI::Write(WRITE_DISABLE);

    HAL_SPI::Write(WRITE_ENABLE);

    int size = 1 + 3 + 256;

    uint8 data[1 + 3 + 256];

    data[0] = PROGRAM_PAGE;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;

    for (int i = 0; i < 256; i++)
    {
        data[4 + i] = buffer[i];
    }

    HAL_SPI::Write(data, size);

    HAL_SPI::Write(WRITE_DISABLE);
}


void W25Q80DV::Read256bytes(uint8 *)
{

}


void W25Q80DV::Write24bit(uint8 command, uint bits24)
{
    uint8 data[4];

    data[0] = command;
    data[1] = (uint8)(bits24 >> 16);
    data[2] = (uint8)(bits24 >> 8);
    data[3] = (uint8)(bits24);

    HAL_SPI::Write(data, 4);
}


bool W25Q80DV::IsBusy()
{
    static const uint8 out[2] = { READ_STATUS_1, 0 };
    static uint8 in[2] = { 0, 0 };

    HAL_SPI::WriteRead(out, in, 2);

    return (in[1] & 0x01);
}


void W25Q80DV::WaitRelease()
{
    while (IsBusy())
    {
    }
}
