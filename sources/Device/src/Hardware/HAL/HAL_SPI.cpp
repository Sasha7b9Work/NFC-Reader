// 2022/6/10 9:12:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


namespace HAL_SPI
{
    bool initialized = false;

    static SPI_HandleTypeDef handle;

    static void Init();

    namespace CS
    {
        static GPIO_TypeDef *PORT_CS = GPIOA;
        static const uint16 PIN_CS_MEMORY = GPIO_PIN_12;
        static const uint16 PIN_CS_READER = GPIO_PIN_15;

        static const uint16 pins[DirectionSPI::Count] = { PIN_CS_MEMORY, PIN_CS_READER };

        void Hi(DirectionSPI::E dir)
        {
            HAL_GPIO_WritePin(PORT_CS, pins[dir], GPIO_PIN_SET);
        }

        void Low(DirectionSPI::E dir)
        {
            HAL_GPIO_WritePin(PORT_CS, pins[dir], GPIO_PIN_RESET);
        }

        void Init()
        {
            GPIO_InitTypeDef is =
            {
                PIN_CS_MEMORY | PIN_CS_READER,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH
            };

            HAL_GPIO_Init(PORT_CS, &is);

            Hi(DirectionSPI::Memory);
            Hi(DirectionSPI::Reader);
        }
    }
}


void HAL_SPI::Init()
{
    if (initialized)
    {
        return;
    }

    initialized = true;

    HAL_I2C1::DeInit();

    __HAL_RCC_SPI1_CLK_ENABLE();

    CS::Init();

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_5 |    // MOSI
        GPIO_PIN_3,     // SCK
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH
    };

    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_4;        // MISO
    is.Mode = GPIO_MODE_INPUT;
    is.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOB, &is);

    __HAL_AFIO_REMAP_SPI1_ENABLE();

    handle.Instance = SPI1;
    handle.Init.Mode = SPI_MODE_MASTER;
    handle.Init.Direction = SPI_DIRECTION_2LINES;
    handle.Init.DataSize = SPI_DATASIZE_8BIT;
    handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
    handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    handle.Init.NSS = SPI_NSS_SOFT;
    handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handle.Init.TIMode = SPI_TIMODE_DISABLE;
    handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    handle.Init.CRCPolynomial = 10;

    HAL_SPI_Init(&handle);
}


void HAL_SPI::DeInit()
{
    if (!initialized)
    {
        return;
    }

    initialized = false;

    HAL_SPI_DeInit(&handle);

    __HAL_AFIO_REMAP_SPI1_DISABLE();

    __HAL_RCC_SPI1_CLK_DISABLE();
}


void HAL_SPI::Write(DirectionSPI::E dir, const void *buffer, int size)
{
    Init();

    CS::Low(dir);

    HAL_SPI_Transmit(&handle, (uint8 *)buffer, (uint16)size, 100);

    CS::Hi(dir);
}


void HAL_SPI::Write(DirectionSPI::E dir, uint8 byte)
{
    Init();

    CS::Low(dir);

    HAL_SPI_Transmit(&handle, &byte, 1, 100);

    CS::Hi(dir);
}


void HAL_SPI::Read(DirectionSPI::E dir, const void *buffer, int size)
{
    Init();

    CS::Low(dir);

    HAL_SPI_Receive(&handle, (uint8 *)buffer, (uint16)size, 100);

    CS::Hi(dir);
}


void HAL_SPI::WriteRead(DirectionSPI::E dir, const void *out, void *in, int size)
{
    Init();

    CS::Low(dir);

    HAL_SPI_TransmitReceive(&handle, (uint8 *)out, (uint8 *)in, (uint16)size, 100);

    CS::Hi(dir);
}
