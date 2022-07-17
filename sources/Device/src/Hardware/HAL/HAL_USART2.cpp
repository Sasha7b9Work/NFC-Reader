// 2022/6/14 22:08:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include "Hardware/Power.h"
#include <stm32f1xx_hal.h>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdarg>


    // PA2     ------> USART2_TX / DI / A / D0 - без инверсии
    // PA3     ------> USART2_RX / RO / B / D1- с инверсией


namespace HAL_USART2_WG26
{
    static Type::E type = Type::None;

    namespace Mode
    {
        // Включить режим передачи
        void Transmit();

        // Включить режим приёма
        void Receive();
    }

    namespace UART
    {
        static UART_HandleTypeDef handle;

        static uint8 buffer = 0;                    // Буфер для передачи данных через UART

        void Init();
    }

    namespace WG26
    {
        void Init();

        void Transmit(CLRC66303HN::UID &);

        namespace D0        // без инверсии
        {
            void Hi();

            void Lo();
        }

        namespace D1        // с инверсией
        {
            void Hi();

            void Lo();
        }
    }

    void *handle = (void *)&UART::handle;
}


void HAL_USART2_WG26::SetType(Type::E _type)
{
    type = _type;

    switch (type)
    {
    case HAL_USART2_WG26::Type::None:
        break;

    case HAL_USART2_WG26::Type::WG26:
        WG26::Init();
        break;

    case HAL_USART2_WG26::Type::UART:
        UART::Init();
        break;
    }

    Mode::Receive();
}


void HAL_USART2_WG26::TransmitRAW(char *message)
{
    Mode::Transmit();

    HAL_UART_Transmit(&UART::handle, (uint8 *)message, (uint16)std::strlen(message), 1000);

    Mode::Receive();
}


void HAL_USART2_WG26::Transmit(char *format, ...)
{
    char message[128];
    std::va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    TransmitRAW(message);
}


void HAL_USART2_WG26::TransmitUID(CLRC66303HN::UID &uid)
{
    switch (type)
    {
    case Type::None:
        break;

    case Type::WG26:
        SetType(Type::WG26);
        WG26::Transmit(uid);
        SetType(Type::UART);
        break;

    case Type::UART:
        Transmit("%s\x0D\x0A", uid.ToString());
        break;
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{
    static char *request = "#MSR\x0D\x0A";

    static int pointer = 0;

    if (HAL_USART2_WG26::UART::buffer == ' ')
    {

    }
    else
    {
        int symbol = std::toupper((int)HAL_USART2_WG26::UART::buffer);

        if (request[pointer] == symbol)
        {
            pointer++;

            if (pointer == (int)std::strlen(request))
            {
                HAL_USART2_WG26::SetType(HAL_USART2_WG26::Type::UART);

                char message[100];

                std::sprintf(message, "OK;%02Xh;%3.1fV;%3.2fg;%3.2fg;%3.2fg;%3.1fC\x0D\x0A",
                    W25Q80DV::TestValue(),
                    HAL_ADC::GetValue(),
                    LIS2DH12::GetAccelerationX().ToAccelearation(),
                    LIS2DH12::GetAccelerationY().ToAccelearation(),
                    LIS2DH12::GetAccelerationZ().ToAccelearation(),
                    LIS2DH12::GetRawTemperature().ToTemperatrue());

                HAL_USART2_WG26::TransmitRAW(message);

                pointer = 0;
            }
        }
        else
        {
            pointer = 0;
        }
    }

    HAL_UART_Receive_IT(&HAL_USART2_WG26::UART::handle, &HAL_USART2_WG26::UART::buffer, 1);
}


void HAL_USART2_WG26::WG26::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_2 |                // A / D0 - без инверсии
        GPIO_PIN_3,                 // B / D1 - с инверсией
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOA, &is);

    D0::Hi();
    D1::Hi();

    HAL_USART2_WG26::Mode::Transmit();
}


void HAL_USART2_WG26::UART::Init()
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef is = { 0 };

    is.Pin = GPIO_PIN_2;        // TX
    is.Mode = GPIO_MODE_AF_PP;
    is.Pull = GPIO_PULLUP;
    is.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = GPIO_PIN_3;        // RX
    is.Mode = GPIO_MODE_INPUT;
    is.Pull = GPIO_NOPULL;
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

    is.Pin = GPIO_PIN_9;                    // Приём/передача
    is.Mode = GPIO_MODE_OUTPUT_PP;
    is.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOA, &is);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_UART_Receive_IT(&handle, &buffer, 1);
}


void HAL_USART2_WG26::WG26::D0::Hi()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}


void HAL_USART2_WG26::WG26::D0::Lo()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}


void HAL_USART2_WG26::WG26::D1::Hi()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
}


void HAL_USART2_WG26::WG26::D1::Lo()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}


void HAL_USART2_WG26::Mode::Transmit()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}


void HAL_USART2_WG26::Mode::Receive()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}
