// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"
#include "Hardware/Timer.h"


int main(void)
{
    Device::Init();

    while (true)
    {
        static TimeMeterMS meter;

        if (meter.ElapsedTime() >= 60)
        {
            meter.Reset();

            Device::Update();
        }
    }
}
