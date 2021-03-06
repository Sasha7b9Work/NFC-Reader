// 2022/6/10 9:08:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Power.h"
#include <stm32f1xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}


void MemManage_Handler(void)
{
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    while (1)
    {
    }
}


void UsageFault_Handler(void)
{
    while (1)
    {
    }
}


void SVC_Handler(void)
{

}


void DebugMon_Handler(void)
{

}


void PendSV_Handler(void)
{

}


void SysTick_Handler(void)
{
    HAL_IncTick();
}


void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler((UART_HandleTypeDef *)HAL_USART2_WG26::handle);
}


void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler((TIM_HandleTypeDef *)Power::handleTIM3);
}

#ifdef __cplusplus
}
#endif
