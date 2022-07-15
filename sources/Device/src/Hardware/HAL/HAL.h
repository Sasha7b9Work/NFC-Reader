// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DirectionSPI
{
    enum E
    {
        Memory,         // Работа с флеш-памятью
        Reader,         // Работа с карт-ридером
        Count
    };
};


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


namespace HAL_FLASH
{
    void LoadAntennaConfiguration106();
}


namespace HAL_SPI
{
    void DeInit();

    void Write(DirectionSPI::E, uint8);
    void Write(DirectionSPI::E, const void *buffer, int size);
    void Read(DirectionSPI::E, const void *buffer, int size);
    void WriteRead(DirectionSPI::E, const void *out, void *in, int size);
}


namespace HAL_I2C1
{
    void DeInit();

    int8 Read(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Read16(uint8 dev_id, uint8 *data);

    int8 Write(uint8 dev_id, uint8 reg_addr, const uint8 *reg_data, uint16 len);
    int8 Write8(uint8 dev_id, uint8 data);
}


namespace HAL_USART2
{
    void Init();

    void TransmitRAW(char *);

    void Transmit(char *format, ...);

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
    void TIM3_IRQHandler(void);
        
#ifdef __cplusplus
}
#endif
