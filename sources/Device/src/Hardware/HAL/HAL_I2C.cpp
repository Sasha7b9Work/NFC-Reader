#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>


static I2C_HandleTypeDef hi2c1;


namespace HAL_I2C1
{
    void *handle = &hi2c1;
}


void HAL_I2C1::Init(void)
{

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c1);
}


void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (i2cHandle->Instance == I2C1)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}


int8 HAL_I2C1::Read(uint8 dev_id, uint8 reg_addr, uint8* reg_data, uint16 len)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, (uint16)(dev_id << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 1);

    if (status == HAL_OK)
    {
        rslt = 0;
    }
    else
    {
        rslt = -1;
    }
    return rslt;
}


int8 HAL_I2C1::Read16(uint8 dev_id, uint8* data)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1, (uint16)((dev_id << 1) + 1), data, 2, 1);

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    return (status == HAL_OK) ? 0 : -1;
}


int8 HAL_I2C1::Write(uint8 dev_id, uint8 reg_addr, uint8* reg_data, uint16 len)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {

    }
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_I2C_Mem_Write(&hi2c1, (uint16)(dev_id << 1), reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 0xffff);

    if (status == HAL_OK)
    {
        rslt = 0;
    }
    else
    {
        rslt = -1;
    }
    return rslt;
}


int8 HAL_I2C1::Write8(uint8 dev_id, uint8 data)
{
    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, (uint16)(dev_id << 1), &data, 1, 10);

    while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    {
    }

    return (status == HAL_OK) ? 0 : -1;
}
