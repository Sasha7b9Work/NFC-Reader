// 2022/6/10 9:12:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


namespace HAL_SPI
{
    bool initialized = false;

    static SPI_HandleTypeDef handle;

    namespace CS
    {
        static GPIO_TypeDef *PORT_CS = GPIOA;
        static const uint16 PIN_CS = GPIO_PIN_12;

        void Hi()
        {
            HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_SET);
        }

        void Low()
        {
            HAL_GPIO_WritePin(PORT_CS, PIN_CS, GPIO_PIN_RESET);
        }

        void Init()
        {
            GPIO_InitTypeDef is =
            {
                PIN_CS,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH
            };

            HAL_GPIO_Init(PORT_CS, &is);

            Hi();
        }
    }
}


void HAL_SPI::Init()
{
    if (initialized)
    {
        return;
    }

    HAL_I2C::DeInit();

    __HAL_RCC_SPI1_CLK_ENABLE();

    CS::Init();

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_4 |    // MISO
        GPIO_PIN_5 |    // MOSI
        GPIO_PIN_3,     // SCK
        GPIO_MODE_AF_PP,
        GPIO_SPEED_FREQ_HIGH
    };

    HAL_GPIO_Init(GPIOB, &is);

    handle.Instance = SPI1;
}


void HAL_SPI::DeInit()
{
    initialized = false;
}
