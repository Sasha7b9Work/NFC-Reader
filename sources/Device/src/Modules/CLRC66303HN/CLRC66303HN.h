// 2022/7/3 11:16:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct UID
{
    UID() : byte0(0), byte1(0), byte2(0), byte3(0), byte4(0) {}

    uint8 byte0;
    uint8 byte1;
    uint8 byte2;
    uint8 byte3;
    uint8 byte4;
};


namespace CLRC66303HN
{
    void Init();

    void Update();

    uint8 GetRegister06();

    BitSet16 GetData();

    UID GetUID();
}
