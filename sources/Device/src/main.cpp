// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Device.h"


/*
* B8, B9 - �����
* B6 - ����� ����������
*/



int main(void)
{
    Device::Init();

    while (true)
    {
        Device::Update();
    }
}
