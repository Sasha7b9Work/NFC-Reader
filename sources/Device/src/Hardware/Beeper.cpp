// 2022/6/10 9:08:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


namespace Beeper
{
    bool running = false;
    
    TimeMeterMS meter;
    
    bool state_one = true;
}


void Beeper::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_7 | GPIO_PIN_8,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH
    };

    HAL_GPIO_Init(GPIOA, &is);
}


void Beeper::Run()
{
    running = true;
}


void Beeper::Stop()
{
    running = false;
}


void Beeper::Update()
{
    if(meter.ElapsedTime() > 0)
    {
        meter.Reset();
        state_one = !state_one;
        
        if(state_one)
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
        }
    }
}
