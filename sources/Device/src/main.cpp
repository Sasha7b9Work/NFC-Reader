// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/Power.h"
#include "Hardware/Beeper.h"
#include <stm32f1xx_hal.h>


int main(void)
{
    Device::Init();

    Power::Init();

    while (true)
    {
        if (!Beeper::Running())
        {
            Power::EnterSleepMode();
        }

        Device::Update();
    }
}
