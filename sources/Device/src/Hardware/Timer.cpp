// 2022/6/10 9:08:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


namespace Timer
{
    static TIM_HandleTypeDef handleTIM2;
    static TIM_HandleTypeDef handleTIM4;
}


void Timer::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handleTIM2.Instance = TIM2;                             // Для отсчёта тиков
    handleTIM2.Init.Prescaler = 0;
    handleTIM2.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM2.Init.Period = 65535;
    handleTIM2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handleTIM2);

    HAL_TIM_Base_Start(&handleTIM2);

    __HAL_RCC_TIM4_CLK_ENABLE();

    handleTIM4.Instance = TIM4;                             // Для отсчёта микросекунд
    handleTIM4.Init.Prescaler = 60;                         // Каждые 60 тиков будет срабатывать - 1 мкс
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM4.Init.Period = 65535;
    handleTIM4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handleTIM4);

    HAL_TIM_Base_Start(&handleTIM4);
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


void TimeMeterMS::WaitFor(uint ms)
{
    while (ElapsedTime() < ms)
    {
    }
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


void TimeMeterUS::Reset()
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
    TIM4->CNT = 0;
    TIM4->CR1 |= TIM_CR1_CEN;
}


void TimeMeterUS::WaitFor(uint us)
{
    while (ElapsedUS() < us)
    {
    }
}


uint TimeMeterUS::ElapsedUS()
{
    return TIM4->CNT;
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
