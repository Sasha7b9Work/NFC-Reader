// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HAL
{
    void Init();
}


namespace HAL_SPI
{
    void Init();

    void DeInit();
}


namespace HAL_I2C
{
    void Init();

    void DeInit();
}


#ifdef __cplusplus
extern "C" {
#endif 

    void NMI_Handler(void);
    void HardFault_Handler(void);
    void MemManage_Handler(void);
    void BusFault_Handler(void);
    void UsageFault_Handler(void);
    void SVC_Handler(void);
    void DebugMon_Handler(void);
    void PendSV_Handler(void);
    void SysTick_Handler(void);
    void USB_LP_CAN1_RX0_IRQHandler(void);

#ifdef __cplusplus
}
#endif
