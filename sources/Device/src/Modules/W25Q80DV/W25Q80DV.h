// 2022/6/10 9:03:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace W25Q80DV
{
    // Размер буфера ограничен 1024 байтами
    void Write1024bytes(const uint8 *buffer, int size);

    // Размер буфера ограничен 1024 байтами
    void Read1024bytes(uint8 *buffer, int size);

    void ReadID();

    bool Test();

    // Возвращает значение, считанное во время теста
    uint8 TestValue();
}
