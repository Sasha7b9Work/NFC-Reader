// 2022/6/14 22:08:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f1xx_hal.h>
#include <cstring>


    // PA2     ------> USART2_TX
    // PA3     ------> USART2_RX


namespace HAL_USART2
{
    UART_HandleTypeDef handle;

    namespace Mode
    {
        // ¬ключить режим передачи
        void Transmit()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
        }

        // ¬ключить режим приЄма
        void Receive()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
        }
    }
}



void HAL_USART2::Init()
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef is = { 0 };

    is.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    is.Mode = GPIO_MODE_AF_PP;
    is.Pull = GPIO_PULLUP;
    is.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &is);

    handle.Instance = USART2;
    handle.Init.BaudRate = 115200;
    handle.Init.WordLength = UART_WORDLENGTH_8B;
    handle.Init.StopBits = UART_STOPBITS_1;
    handle.Init.Parity = UART_PARITY_NONE;
    handle.Init.Mode = UART_MODE_TX_RX;
    handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handle.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&handle);

    is.Pin = GPIO_PIN_9;
    is.Mode = GPIO_MODE_OUTPUT_PP;
    is.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &is);
}


void HAL_USART2::Transmit(char *message)
{
    Mode::Transmit();

    HAL_UART_Transmit(&handle, (uint8 *)message, (uint16)(std::strlen(message) + 1), 1000);
}
