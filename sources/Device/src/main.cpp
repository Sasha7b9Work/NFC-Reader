// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"
#include "Hardware/Power.h"
#include <stm32f1xx_hal.h>


int main(void)
{
    Device::Init();

    Power::Init();

    while (true)
    {
        Power::EnterSleepMode();

        Device::Update();
    }
}
