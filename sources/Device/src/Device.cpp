// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include <cstdlib>
#include <cstring>


static uint8 buffer_write[256];
static uint8 buffer_read[256];


void Device::Init()
{
    HAL::Init();

    Timer::Delay(500);

    for (int i = 0; i < 256; i++)
    {
        buffer_write[i] = (uint8)std::rand();
    }

    W25Q80DV::Write(buffer_write, 256);

    W25Q80DV::Read(buffer_read, 256);

    if (std::memcmp(buffer_write, buffer_read, 256) == 0)
    {
        Beeper::Init();
        Beeper::Run();
    }
}


void Device::Update()
{
    Beeper::Update();
}
