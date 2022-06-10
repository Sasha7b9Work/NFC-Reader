// 2022/6/10 9:12:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"


namespace HAL_I2C
{
    bool initialized = false;
}


void HAL_I2C::Init()
{
    if (initialized)
    {
        return;
    }

    HAL_SPI::DeInit();


}


void HAL_I2C::DeInit()
{
    initialized = false;
}
