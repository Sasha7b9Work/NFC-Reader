// 2022/6/24 15:52:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StructDataRaw
{
    union
    {
        union
        {
            uint8 lo;
            uint8 hi;
        };
        int16 raw;
    };

    float ToTemperatrue()
    {
        return 25.0f + (float)raw / 256.0f;
    }

    float ToAccelearation()
    {
        return (float)raw / 16.0f;
    }
};



namespace LIS2DH12
{
    void Init();

    void Update();

    StructDataRaw GetAccelerationX();

    StructDataRaw GetAccelerationY();

    StructDataRaw GetAccelerationZ();

    StructDataRaw GetRawTemperature();
}
