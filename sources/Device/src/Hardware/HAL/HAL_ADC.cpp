// 2022/6/17 0:37:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


namespace HAL_ADC
{
    static ADC_HandleTypeDef handle;
}


void HAL_ADC::Init()
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_0,
        GPIO_MODE_ANALOG,
        0,
        0
    };

    HAL_GPIO_Init(GPIOA, &is);


    ADC_ChannelConfTypeDef sConfig = { 0 };

    handle.Instance = ADC1;
    handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.NbrOfConversion = 1;

    HAL_ADC_Init(&handle);

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

    HAL_ADC_ConfigChannel(&handle, &sConfig);

    HAL_ADCEx_Calibration_Start(&handle);
}


uint HAL_ADC::GetValue()
{
    HAL_ADC_Start(&handle);

    HAL_ADC_PollForConversion(&handle, 10);

    return HAL_ADC_GetValue(&handle);
}
