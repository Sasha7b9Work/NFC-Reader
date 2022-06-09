// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"


/*
* B8, B9 - мотор
* B6 - сброс напряжения
*/



int main(void)
{
    Device::Init();

    while (true)
    {
        Device::Update();
    }
}
