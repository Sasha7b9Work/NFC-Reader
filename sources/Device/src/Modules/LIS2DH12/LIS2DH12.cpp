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
    static StructRawTemp raw_temp;

    // Initialize mems driver interface
    static stmdev_ctx_t dev_ctx;

    static int16_t data_raw_acceleration[3];
    static int16_t data_raw_temperature;
    static float acceleration_mg[3] = { 0.0f, 0.0f, 0.0f };

    static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);

    static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);

    static void Write(uint8 reg, uint8 data)
    {
        HAL_I2C1::Write(0x19, reg, &data, 1);
    }
}


void LIS2DH12::Init()
{
    dev_ctx.write_reg = platform_write;
    dev_ctx.read_reg = platform_read;

    // Check device ID
    uint8 whoamI = 0;
    lis2dh12_device_id_get(&dev_ctx, &whoamI);

    if (whoamI != LIS2DH12_ID) {
        while (1) {
            /* manage here device not found */
        }
    }

    // Enable Block Data Update.
//    lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    uint8 data = 0;
    HAL_I2C1::Read(0x19, LIS2DH12_CTRL_REG4, &data, 1);
    data |= (1 << 7);
    HAL_I2C1::Write(0x19, LIS2DH12_CTRL_REG4, &data, 1);

    // Set Output Data Rate to 1Hz.
//    lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_1Hz);

    HAL_I2C1::Read(0x19, LIS2DH12_CTRL_REG1, &data, 1);
    data |= (1 << 4);
    HAL_I2C1::Write(0x19, LIS2DH12_CTRL_REG1, &data, 1);

    // Set full scale to 2g.
//    lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);

    // Enable temperature sensor.
    //lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);
    data = 0xC0;
    HAL_I2C1::Write(0x19, LIS2DH12_TEMP_CFG_REG, &data, 1);

    // Set device in continuous mode with 12 bit resol.
//    lis2dh12_operating_mode_set(&dev_ctx, LIS2DH12_HR_12bit);

    HAL_I2C1::Read(0x19, LIS2DH12_CTRL_REG4, &data, 1);
    data |= (1 << 3);
    Write(LIS2DH12_CTRL_REG4, data);
}


void LIS2DH12::Update()
{
    lis2dh12_reg_t reg;

    lis2dh12_temp_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte)
    {
        Timer::Delay(1);

        HAL_I2C1::Read(0x19, LIS2DH12_OUT_TEMP_L, &raw_temp.lo, 1);
        HAL_I2C1::Read(0x19, LIS2DH12_OUT_TEMP_H, &raw_temp.hi, 1);
    }
}


StructRawTemp LIS2DH12::GetRawTemperature()
{
    return raw_temp;
}


float LIS2DH12::GetAccelerationX()
{
    return acceleration_mg[0];
}


float LIS2DH12::GetAccelerationY()
{
    return acceleration_mg[1];
}


float LIS2DH12::GetAccelerationZ()
{
    return acceleration_mg[2];
}


/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
static int32_t LIS2DH12::platform_write(void * /*handle*/, uint8_t reg, const uint8_t *buf, uint16_t len)
{
    return (int)HAL_I2C1::Write(0x19, reg, buf, len);
}


/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to reads
 *
 */
static int32_t LIS2DH12::platform_read(void * /*handle*/, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return (int)HAL_I2C1::Read(0x19, reg, buf, len);
}
