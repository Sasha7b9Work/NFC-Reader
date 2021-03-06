// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Modules/WS2812B/WS2812B.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include <stm32f1xx_hal.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>


void Device::Init()
{
    HAL::Init();

    Timer::Init();

    Timer::Delay(500);

    if (W25Q80DV::Test())
    {
        Beeper::Beep(2000, 500);
    }
    else
    {
        Beeper::Beep(2000, 5000);
    }

    WS2812B::Init();

    LIS2DH12::Init();

    CLRC66303HN::Init();
}


void Device::Update()
{
    HAL_ADC::Update();          // ???????? ??????????

    WS2812B::Update();          // ??????????

    LIS2DH12::Update();         // ????????????

    CLRC66303HN::Update();      // ????-?????

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET)
    {
        Beeper::Start(2000);
    }
    else
    {
        Beeper::Stop();
    }
}
