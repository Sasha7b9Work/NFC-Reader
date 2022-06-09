// 2022/6/7 9:06:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ADC
{
    void Init();

    bool DataReady();

    float GetVoltage();

    void Reset();
}
