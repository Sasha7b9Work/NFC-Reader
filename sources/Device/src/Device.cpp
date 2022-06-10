// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include <cstdlib>
#include <cstring>


static bool TestFlashMemory();


void Device::Init()
{
    HAL::Init();

    Timer::Delay(500);

    Beeper::Init();
    Beeper::Run();

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
    Beeper::Update();
}


static bool TestFlashMemory()
{
    static uint8 buffer_write[256];
    static uint8 buffer_read[256];

    for (int i = 0; i < 256; i++)
    {
        buffer_write[i] = (uint8)std::rand();
    }

    W25Q80DV::Write1024bytes(buffer_write, 256);

    W25Q80DV::Read1024bytes(buffer_read, 256);

    return (std::memcmp(buffer_write, buffer_read, 256) == 0);
}
