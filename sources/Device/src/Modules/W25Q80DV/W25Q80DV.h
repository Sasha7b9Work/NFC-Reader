// 2022/6/10 9:03:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace W25Q80DV
{
    void Write(uint address, void *buffer);

    void Write(uint address, uint8 byte);
}