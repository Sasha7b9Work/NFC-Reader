// 2022/6/15 22:18:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/WS2812B/WS2812B.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>


namespace WS2812B
{
    struct Color
    {
        enum E
        {
            None,
            RED,
            GREEN,
            BLUE,
            BLACK
        };
    };

    static Color::E color = Color::None;

    uint dT0H = 18;     // В тактах
    uint dT0L = 47;     // В тактах
    uint dT1H = 47;     // В тактах
    uint dT1L = 19;     // В тактах

    void FireRED();
    void FireGREEN();
    void FireBLUE();
    void FireBLACK();

    void Fire(uint8 red, uint8 green, uint8 blue);

    namespace DLED
    {
        void Init()
        {
            GPIO_InitTypeDef is =
            {
                GPIO_PIN_1,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH
            };

            HAL_GPIO_Init(GPIOB, &is);

            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        }

        // Записать полную последовательность бит в четыре индикатора
        // 96 бит = 4 индикатора по 24 бита
        // data представляет собой массив количества тактов, в которые нужно передёргивать из 0 в 1 и обратно.
        // В момент времени 0 нужно передёрнуть в 1. Далее идут пары чисел, соответствующие переключению в 0 и в единицу
        // data[0] - окончание положительного импульса 0-го бита
        // data[1] - окончание отрицательного импульса 0-го бита
        // data[2], data[3] - бит 1
        // data[4], data[5] - бит 2
        // В массиве не 96 * 2 элементов, а на 1 больше - заканчивается нулём.
        // 24 - бит на индикатор, 2 - интервалов на бит, 4 - количество индикаторов, 1 - завершающий ноль
        void WriteFullSequency(uint data[24 * 2 * 4 + 1]);
    }
}


void WS2812B::Init()
{
    DLED::Init();
}


void WS2812B::Update()
{
    bool stateLR = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == GPIO_PIN_SET;
    bool stateLG = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET;

    if (!stateLR && stateLG)
    {
        FireRED();
    }
    else if (stateLR && !stateLG)
    {
        FireGREEN();
    }
    else if (!stateLR && !stateLG)
    {
        FireBLUE();
    }
    else if (stateLR && stateLG)
    {
        FireBLACK();
    }
}


void WS2812B::FireRED()
{
    if (color == Color::RED)
    {
        return;
    }

    color = Color::RED;

    //Fire(127, 0, 0);
    Fire(255, 136, 0);      // Оранжевый
}


void WS2812B::FireGREEN()
{
    if (color == Color::GREEN)
    {
        return;
    }

    color = Color::GREEN;

    //Fire(0, 127, 0);
    Fire(0, 191, 255);      // Синий
}


void WS2812B::FireBLUE()
{
    if (color == Color::BLUE)
    {
        return;
    }

    color = Color::BLUE;

    Fire(0, 0, 127);
}


void WS2812B::FireBLACK()
{
    if (color == Color::BLACK)
    {
        return;
    }

    color = Color::BLACK;

    Fire(0, 0, 0);
}


void WS2812B::Fire(uint8 red, uint8 green, uint8 blue)
{
    /*
    * Цвет хранится в формате GRB
    */

#define ADD_AND_WRITE(delta)        \
    full_time += delta;             \
    *time++ = full_time

    uint value = (uint)((green << 16) | (red << 8) | blue);

    uint bits[24 * 2 * 4 + 1];
    uint *time = &bits[0];
    uint full_time = 0;

    for (int indicator = 0; indicator < 4; indicator++)
    {
        uint col = (value << 8);

        for (int i = 0; i < 24; i++)
        {
            if (col & 0x80000000)     // единица
            {
                ADD_AND_WRITE(dT1H);
                ADD_AND_WRITE(dT1L);
            }
            else                        // ноль
            {
                ADD_AND_WRITE(dT0H);
                ADD_AND_WRITE(dT0L);
            }

            col <<= 1;
        }
    }

    *time = 0;

    DLED::WriteFullSequency(bits);
}


void WS2812B::DLED::WriteFullSequency(uint data[24 * 2 * 4 + 1])
{
#define PIN_SET      GPIOB->BSRR = GPIO_PIN_1
#define PIN_RESET    GPIOB->BSRR = (uint)(GPIO_PIN_1 << 16)

    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;

    while (*data)
    {
        PIN_SET;

        while(TIM2->CNT < *data) {}
        data++;

        PIN_RESET;

        while(TIM2->CNT < *data) {}
        data++;
    }
}
