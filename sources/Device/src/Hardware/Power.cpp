// 2022/6/30 23:43:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Power.h"
#include <stm32f1xx_hal.h>


namespace Power
{
    static TIM_HandleTypeDef handle =
    {
        TIM3,
        {
            (uint)(60000 / 17 - 1),         // prescaler, 50?? = 20??
            TIM_COUNTERMODE_UP,
            999,                            // period
            TIM_CLOCKDIVISION_DIV1,
            0,                              // RepetitionCounter
            TIM_AUTORELOAD_PRELOAD_DISABLE  // AutoReloadPreload
        }
    };

    void *handleTIM3 = &handle;

    static int number = 0;
}


void Power::Init()
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_TIM_Base_Init(&handle);

    HAL_TIM_Base_Start_IT(&handle);
}


void Power::EnterSleepMode()
{
    HAL_SuspendTick();

    HAL_PWR_DisableSleepOnExit();

    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);   // ????? ????????? ???????????????

    HAL_ResumeTick();                                                   // ? ????? ???????????? ?????? ????? ?????? ??????????
}


int Power::GetNumber()
{
    return number;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *)
{
    Power::number++;
}
