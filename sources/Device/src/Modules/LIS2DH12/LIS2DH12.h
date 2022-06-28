// 2022/6/24 15:52:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct StructRawTemp
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

    float ToAbs()
    {
        return 25.0f + (float)hi + (float)lo / 256.0f;
    }

};



namespace LIS2DH12
{
    void Init();

    void Update();

    float GetAccelerationX();

    float GetAccelerationY();

    float GetAccelerationZ();

    StructRawTemp GetRawTemperature();
}
