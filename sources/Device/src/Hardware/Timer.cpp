// 2022/6/10 9:08:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


namespace Timer
{
    static TIM_HandleTypeDef handle;
}


void Timer::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handle.Instance = TIM2;
    handle.Init.Prescaler = 0;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle.Init.Period = 65535;
    handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handle);

    HAL_TIM_Base_Start(&handle);
}


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


void TimeMeterTics::Reset()
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}


void TimeMeterTics::WaitFor(uint ticks)
{
    while (TIM2->CNT < ticks)
    {
    }
}
