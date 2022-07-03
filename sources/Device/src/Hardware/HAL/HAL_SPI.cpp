// 2022/6/15 17:05:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


namespace HAL_SPI
{
    bool initialized = false;

    void WriteByte(uint8);

    uint8 WriteReadByte(uint8);

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

    namespace CLK
    {
        void Hi()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
        }

        void Low()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
        }
    }

    struct MOSI
    {
        static void SetForHiBit(uint8 byte)
        {
            if (byte & 0x80)
            {
                Hi();
            }
            else
            {
                Low();
            }
        }

    private:

        static void Hi()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        }

        static void Low()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        }
    };

    namespace MISO
    {
        int State()
        {
            return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_SET ? 1 : 0;
        }
    }
}


void HAL_SPI::Init()
{
    if (initialized)
    {
        return;
    }

    HAL_I2C1::DeInit();

    initialized = true;

    GPIO_InitTypeDef is =
    {
        GPIO_PIN_3 |    // SCK
        GPIO_PIN_5,     // MOSI
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_HIGH
    };

    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_4;    // MISO
    is.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(GPIOB, &is);

    CS::Init();

    MOSI::SetForHiBit(0xff);
    CLK::Hi();
}


void HAL_SPI::Write(const void *buffer, int size)
{
    CS::Low();

    uint8 *bytes = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        WriteByte(*bytes++);
    }

    CS::Hi();
}


void HAL_SPI::WriteRead(const void *out, void *in, int size)
{
    CS::Low();

    uint8 *write = (uint8 *)out;
    uint8 *read = (uint8 *)in;

    for (int i = 0; i < size; i++)
    {
        *read++ = WriteReadByte(*write++);
    }

    CS::Hi();
}


void HAL_SPI::Write(uint8 byte)
{
    Write((const void *)&byte, 1);
}


void HAL_SPI::WriteByte(uint8 byte)
{
    for (int i = 0; i < 8; i++)
    {
        CLK::Low();

        MOSI::SetForHiBit(byte);

        CLK::Hi();

        MOSI::SetForHiBit(byte);

        byte <<= 1;
    }
}


uint8 HAL_SPI::WriteReadByte(uint8 byte)
{
    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        CLK::Low();

        MOSI::SetForHiBit(byte);

        byte <<= 1;

        CLK::Hi();

        result <<= 1;

        if (MISO::State())
        {
            result |= 0x01;
        }
    }

    return result;
}
