// 2022/6/24 15:52:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace LIS2DH12
{
    void Init();

    void Update();

    float GetAccelerationX();

    float GetAccelerationY();

    float GetAccelerationZ();

    float GetTemperature();

    uint8 GetByte();
}
