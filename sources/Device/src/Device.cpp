// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Modules/WS2812B/WS2812B.h"
#include "Hardware/ADC.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>


static bool TestFlashMemory();


void Device::Init()
{
    HAL::Init();

    Timer::Init();

    Timer::Delay(500);

    Beeper::Init();
    Beeper::Run();

    ADC::Init();

    TimeMeterMS meter;

    while (meter.ElapsedTime() < 1000)
    {
        Beeper::Update();
    }

    if (!TestFlashMemory())
    {
        Beeper::Stop();
    }
}


void Device::Update()
{
    static TimeMeterMS meter;

    Beeper::Update();

    if (meter.ElapsedTime() > 1000)
    {
        meter.Reset();

        W25Q80DV::ReadID();

        char message[100];

        std::sprintf(message, "Value ADC : %d", ADC::GetValue());

        HAL_USART2::Transmit(message);
    }
}


static bool TestFlashMemory()
{
    uint8 out[256] = { 0x55 };
    uint8 in[256] = { 0x00 };

    W25Q80DV::Write1024bytes(out, 1);

    W25Q80DV::Read1024bytes(in, 1);

    return (std::memcmp(out, in, 1) == 0);
}
