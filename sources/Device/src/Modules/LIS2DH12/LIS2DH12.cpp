// 2022/6/24 15:52:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Modules/LIS2DH12/LIS2DH12_reg.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>
#include <cstdio>


namespace LIS2DH12
{
    // Initialize mems driver interface
    static stmdev_ctx_t dev_ctx;

    static int16_t data_raw_acceleration[3];
    static int16_t data_raw_temperature;
    static float acceleration_mg[3];
    static float temperature_degC;
    static uint8_t tx_buffer[1000];

    static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);

    static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
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
    lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);

    // Set Output Data Rate to 1Hz.
    lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_1Hz);

    // Set full scale to 2g.
    lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);

    // Enable temperature sensor.
    lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);

    // Set device in continuous mode with 12 bit resol.
    lis2dh12_operating_mode_set(&dev_ctx, LIS2DH12_HR_12bit);
}


void LIS2DH12::Update()
{
    lis2dh12_reg_t reg;
    /* Read output only if new value available */
    lis2dh12_xl_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte)
    {
        /* Read accelerometer data */
        std::memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
        lis2dh12_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
        acceleration_mg[0] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[0]);
        acceleration_mg[1] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[1]);
        acceleration_mg[2] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration[2]);

        std::sprintf((char *)tx_buffer, "Acceleration [mg]:%4.2f\t%4.2f\t%4.2f\r\n", acceleration_mg[0], acceleration_mg[1], acceleration_mg[2]);
//        tx_com(tx_buffer, std::strlen((char const *)tx_buffer));
    }

    lis2dh12_temp_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte)
    {
        /* Read temperature data */
        std::memset(&data_raw_temperature, 0x00, sizeof(int16_t));
        lis2dh12_temperature_raw_get(&dev_ctx, &data_raw_temperature);
        temperature_degC = lis2dh12_from_lsb_hr_to_celsius(data_raw_temperature);

        std::sprintf((char *)tx_buffer, "Temperature [degC]:%6.2f\r\n", temperature_degC);
//        tx_com(tx_buffer, strlen((char const *)tx_buffer));
    }
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
