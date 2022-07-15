// 2022/6/18 10:53:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f1xx_hal.h>


void HAL_PINS::Init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef is =                                   //
    {                                                       //
        GPIO_PIN_1  | GPIO_PIN_5,                           //
        GPIO_MODE_OUTPUT_PP,                                //
        GPIO_NOPULL                                         //
    };                                                      //
                                                            //
    HAL_GPIO_Init(GPIOA, &is);                              //
                                                            //
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);   //
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);   //
                                                            //
    is.Pin = GPIO_PIN_1;                                    //
                                                            //
    HAL_GPIO_Init(GPIOD, &is);                              //
                                                            //
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);   //

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

    //                                  IRQ_SNS INT1 от акселерометра

    is2.Pin = GPIO_PIN_0;
    is2.Pull = GPIO_PULLDOWN;

    HAL_GPIO_Init(GPIOD, &is2);
    uint data = GPIOD->ODR;
    _CLEAR_BIT(data, 0);
    GPIOD->ODR = data;              // Записываем ноль в ODR0

    __HAL_AFIO_REMAP_PD01_ENABLE();

    //                                  IRQ_TRX IRQ от CLRC663

    is2.Pin = GPIO_PIN_4;

    HAL_GPIO_Init(GPIOA, &is2);
    data = GPIOA->ODR;
    _CLEAR_BIT(data, 4);            // Записываем ноль в  ORD4
    GPIOA->ODR = data;
}
