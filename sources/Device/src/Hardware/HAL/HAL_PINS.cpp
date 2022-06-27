// 2022/6/18 10:53:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f1xx_hal.h>


void HAL_PINS::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
        GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_6  | GPIO_PIN_7  |
        GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 |
        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;

    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    is.Pin = GPIO_PIN_All;

    HAL_GPIO_Init(GPIOD, &is);

    GPIO_InitTypeDef is2 =
    {
        GPIO_PIN_10 |               // Сигнал LR
        GPIO_PIN_11,                // Сигнал SND
        GPIO_MODE_INPUT,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH
    };

    HAL_GPIO_Init(GPIOA, &is2);

    is2.Pin = GPIO_PIN_0;           // Сигнал LG

    HAL_GPIO_Init(GPIOB, &is2);

    is2.Pin = GPIO_PIN_0;

    HAL_GPIO_Init(GPIOD, &is2);     // IRQ_SNS INT1 от акселерометра
}
