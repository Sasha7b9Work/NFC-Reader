// 2022/6/30 23:43:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Power
{
    void EnterSleepMode();

    void LeaveSleepMode();

    extern void *handleTIM3;       // TIM_HandleTypeDef
}
