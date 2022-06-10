// 2022/6/10 9:03:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace W25Q80DV
{
    void Write256bytes(uint8 *buffer256);

    void Read256bytes(uint8 *buffer256);
}
