// 2022/6/24 7:00:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Hardware/HAL/HAL.h"


uint8 LIS2DH12::GetID(uint8 dev_id)
{
    uint8 result = 0;

    HAL_I2C1::Read(dev_id, 0x0F, &result, 1);

    return result;
}
