// 2022/6/24 15:52:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Modules/LIS2DH12/LIS2DH12_reg.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f1xx_hal.h>
#include <cstring>
#include <cstdio>


namespace LIS2DH12
{
    static StructDataRaw raw_temp;
    static StructDataRaw raw_acce_x;
    static StructDataRaw raw_acce_y;
    static StructDataRaw raw_acce_z;

    static void Write(uint8 reg, uint8 data)
    {
        HAL_I2C1::Write(0x19, reg, &data, 1);
    }

    static uint8 Read(uint8 reg)
    {
        uint8 result = 0;
        HAL_I2C1::Read(0x19, reg, &result, 1);
        return result;
    }
}


void LIS2DH12::Init()
{
    uint8 data = 0;
    _SET_BIT(data, 4);                              // I1_ZYXDA = 1 - ????????? ?????????? INT1 ?? ?????????? ??????????
    Write(LIS2DH12_CTRL_REG3, data);

    // Enable Block Data Update.
    data = Read(LIS2DH12_CTRL_REG4);
    data |= (1 << 7);                               // BDU = 1
    Write(LIS2DH12_CTRL_REG4, data);

    // Set Output Data Rate to 1Hz.
    HAL_I2C1::Read(0x19, LIS2DH12_CTRL_REG1, &data, 1);
    data |= (1 << 4);                               // ODR = 0b0001, 1Hz
    HAL_I2C1::Write(0x19, LIS2DH12_CTRL_REG1, &data, 1);

    // Set full scale to 2g.
//    lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);   ??? ???????? ?? ?????????

    // Enable temperature sensor.
    data = 0xC0;
    HAL_I2C1::Write(0x19, LIS2DH12_TEMP_CFG_REG, &data, 1);     // TEMP_EN = 0b11

    // Set device in continuous mode with 12 bit resol.
    HAL_I2C1::Read(0x19, LIS2DH12_CTRL_REG4, &data, 1);
    data |= (1 << 3);                                           // HR = 1, (LPen = 0 - High resolution mode)
    Write(LIS2DH12_CTRL_REG4, data);
}


void LIS2DH12::Update()
{
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0) == GPIO_PIN_SET)           // ZYXDA
    {
        if (Read(LIS2DH12_STATUS_REG) & (1 << 3))
        {
            raw_acce_x.lo = Read(LIS2DH12_OUT_X_L);
            raw_acce_x.hi = Read(LIS2DH12_OUT_X_H);

            raw_acce_y.lo = Read(LIS2DH12_OUT_Y_L);
            raw_acce_y.hi = Read(LIS2DH12_OUT_Y_H);

            raw_acce_z.lo = Read(LIS2DH12_OUT_Z_L);
            raw_acce_z.hi = Read(LIS2DH12_OUT_Z_H);
        }
    }

    if (Read(LIS2DH12_STATUS_REG_AUX) & (1 << 2))       // TDA
    {
        raw_temp.lo = Read(LIS2DH12_OUT_TEMP_L);
        raw_temp.hi = Read(LIS2DH12_OUT_TEMP_H);
    }
}


StructDataRaw LIS2DH12::GetRawTemperature()
{
    return raw_temp;
}


StructDataRaw LIS2DH12::GetAccelerationX()
{
    return raw_acce_x;
}


StructDataRaw LIS2DH12::GetAccelerationY()
{
    return raw_acce_y;
}


StructDataRaw LIS2DH12::GetAccelerationZ()
{
    return raw_acce_z;
}
