// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


void Device::Init()
{
    HAL::Init();

    Timer::Delay(500);
}


void Device::Update()
{
}
