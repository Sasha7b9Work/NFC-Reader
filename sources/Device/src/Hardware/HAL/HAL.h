// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace HAL
{
    void Init();
}


namespace HAL_ADC
{
    void Init();

    void Update();

    // Возвращает напряжение в вольтах
    float GetValue();
}


namespace HAL_SPI
{
    void Init();

    void DeInit();

    void Write(uint8);
    void Write(const void *buffer, int size);
    void Read(const void *buffer, int size);
    void WriteRead(const void *out, void *in, int size);
}


namespace HAL_I2C1
{
    void Init();

    int8 Read(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Read16(uint8 dev_id, uint8 *data);

    int8 Write(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Write8(uint8 dev_id, uint8 data);
}


namespace HAL_USART2
{
    void Init();

    void Transmit(char *);

    extern void *handle;       // UART_HandleTypeDef
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
    void USART2_IRQHandler(void);

#ifdef __cplusplus
}
#endif
