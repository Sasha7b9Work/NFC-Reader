// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/Power.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"


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
