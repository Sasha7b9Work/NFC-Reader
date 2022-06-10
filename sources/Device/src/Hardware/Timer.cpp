// 2022/6/10 9:08:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


uint Timer::CurrentTime()
{
    return HAL_GetTick();
}


void Timer::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}


TimeMeterMS::TimeMeterMS()
{
    Reset();
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
    time_pause = 0;
}


void TimeMeterMS::Pause()
{
    time_pause = TIME_MS;
}


void TimeMeterMS::Continue()
{
    time_reset += (TIME_MS - time_pause);
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}
