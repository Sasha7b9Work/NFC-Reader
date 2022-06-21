// 2022/6/19 6:32:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


namespace Beeper
{
    TIM_HandleTypeDef handle;
    int frequency = 0;          // ������� ��������� � ������ ����� �����
    bool running = false;       // ���� true, �� ���� ������
}


void Beeper::Beep(int _frequency, uint timeMS)
{
    Start(_frequency);

    Timer::Delay(timeMS);

    Stop();
}


void Beeper::Start(int _frequency)
{
    if (running && (_frequency == frequency))
    {
        return;
    }

    if (running)
    {
        Stop();
    }

    frequency = _frequency;

    __HAL_RCC_TIM1_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = { 0 };
    TIM_OC_InitTypeDef sConfigOC = { 0 };
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

    handle.Instance = TIM1;
    handle.Init.Prescaler = (uint)(60000 / frequency - 1);
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle.Init.Period = 999;
    handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle.Init.RepetitionCounter = 0;
    handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(&handle);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    HAL_TIMEx_MasterConfigSynchronization(&handle, &sMasterConfig);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 500;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_ConfigChannel(&handle, &sConfigOC, TIM_CHANNEL_1);

    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

    HAL_TIMEx_ConfigBreakDeadTime(&handle, &sBreakDeadTimeConfig);

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_7 | GPIO_PIN_8,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_LOW
    };

    HAL_GPIO_Init(GPIOA, &is);

    __HAL_AFIO_REMAP_TIM1_PARTIAL();

    HAL_TIM_PWM_Start(&handle, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&handle, TIM_CHANNEL_1);

    running = true;
}


void Beeper::Stop()
{
    HAL_TIM_PWM_Stop(&handle, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(&handle, TIM_CHANNEL_1);

    __HAL_RCC_TIM1_CLK_DISABLE();

    running = false;
}
