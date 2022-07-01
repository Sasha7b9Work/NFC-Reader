// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"
#include "Hardware/Timer.h"
#include "Hardware/Power.h"


int main(void)
{
    Device::Init();

    while (true)
    {
        Power::EnterSleepMode();

        Device::Update();
    }
}
