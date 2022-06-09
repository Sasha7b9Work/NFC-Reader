// 2022/6/7 9:07:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/ADC.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


/*
* Полярность?
*/


namespace ADC
{
//#define PORT_RESET  GPIOB           // Сброс напряжения
//#define PIN_RESET   GPIO_PIN_6

#define PORT_READY GPIOB            // АЦП готовность данных
#define PIN_READY GPIO_PIN_14
#define READY PORT_READY, PIN_READY

#define PORT_CLK GPIOB              // АЦП такты
#define PIN_CLK GPIO_PIN_13
#define CLK PORT_CLK, PIN_CLK

#define PORT_IN GPIOB               // АЦП вход. Сюда записываем данные
#define PIN_IN GPIO_PIN_12
#define IN PORT_IN, PIN_IN

#define PORT_OUT GPIOB              // АЦП выход. Отсюда читаем данные
#define PIN_OUT GPIO_PIN_15
#define OUT PORT_OUT, PIN_OUT

#define PIN_CLK_SET   HAL_GPIO_WritePin(CLK, GPIO_PIN_SET)      ; Delay()
#define PIN_CLK_RESET HAL_GPIO_WritePin(CLK, GPIO_PIN_RESET)    ; Delay()

#define PIN_IN_SET    HAL_GPIO_WritePin(IN, GPIO_PIN_SET)       ; Delay()
#define PIN_IN_RESET  HAL_GPIO_WritePin(IN, GPIO_PIN_RESET)     ; Delay()

    void WriteByte(uint8);

    uint8 ReadByte();

    void Delay()
    {
        volatile int i = 0;

        while (i < 100)
        {
            i++;
        }
    }
}


void ADC::Init()
{
    GPIO_InitTypeDef is =
    {
        PIN_READY,
        GPIO_MODE_INPUT,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_HIGH
    };

    HAL_GPIO_Init(PORT_READY, &is);

    is.Pin = PIN_CLK;
    is.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_CLK, &is);

    is.Pin = PIN_IN;
    is.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_IN, &is);

    is.Pin = PIN_OUT;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PORT_OUT, &is);

    is.Pin = GPIO_PIN_13;
    is.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &is);

    PIN_CLK_SET;

//    WriteByte(0x10);
//    WriteByte(0x02);
//    WriteByte(0x20);
//    WriteByte(0x04);

    WriteByte(0x20);
    WriteByte(0x0c);
    WriteByte(0x10);
    WriteByte(0x40);

    PIN_IN_RESET;

//    WriteByte(0x60);
//    WriteByte(0x14);
//    WriteByte(0xF2);
//    WriteByte(0x44);
//    WriteByte(0x70);
//    WriteByte(0xAD);
//    WriteByte(0xE4);
//    WriteByte(0xBF);
}


bool ADC::DataReady()
{
    bool ready = HAL_GPIO_ReadPin(READY) == GPIO_PIN_RESET;

    static TimeMeterMS meter;

    if (meter.ElapsedTime() > 100)
    {
        meter.Reset();
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, ready ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    return ready;
}


float ADC::GetVoltage()
{
    WriteByte(0x38);

    float result = (float)((ReadByte() << 8) + ReadByte());

    Timer::Delay(10);

    return result;
}


void ADC::Reset()
{
//    HAL_GPIO_WritePin(PORT_RESET, PIN_RESET, GPIO_PIN_SET);

    HAL_Delay(1000);

//    HAL_GPIO_WritePin(PORT_RESET, PIN_RESET, GPIO_PIN_RESET);
}


void ADC::WriteByte(uint8 byte)
{
    for (int i = 0; i < 8; i++)
    {
        PIN_CLK_RESET;

        if (byte & 0x80)
        {
            PIN_IN_SET;
        }
        else
        {
            PIN_IN_RESET;
        }

        byte <<= 1;

        PIN_CLK_SET;
    }
}


uint8 ADC::ReadByte()
{
    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        PIN_CLK_RESET;

        result <<= 1;

        if (HAL_GPIO_ReadPin(OUT) == GPIO_PIN_SET)
        {
            result |= 0x01;
        }

        PIN_CLK_SET;
    }

    return result;
}
