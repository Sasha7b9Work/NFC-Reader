﻿// 2022/6/14 22:08:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/LIS2DH12/LIS2DH12.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Modules/CLRC66303HN/CLRC66303HN.h"
#include "Hardware/Timer.h"
#include "Hardware/Power.h"
#include <stm32f1xx_hal.h>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdarg>


    // PA2     ------> USART2_TX / Сюда подаётся последовательность в режиме WG26
    // PA3     ------> USART2_RX


namespace HAL_USART2_WG26
{
    static Type::E type = Type::None;

    namespace Mode
    {
        // Включить режим передачи
        void Transmit();

        // Включить режим приёма
        void Receive();

        // Здесь в режиме WG мы вибираем режим бита либо режим между битами
        namespace WG
        {
            // Уровень, соотвествующий передаче бита
            void Bit();

            // Уровень, соотвествующий промежутку между битами
            void 
        }
    }

    namespace UART
    {
        static UART_HandleTypeDef handle;

        static uint8 buffer = 0;                    // Буфер для передачи данных через UART

        void Init();
    }

    namespace WG26
    {
        /*
        *  Старшие байты первые.
        *  Старшие биты первые.
        *  Длительность бита 100 мкс.
        *  Расстояние между битами 1 мс.
        *  1-й контрольный бит:
        *       если сложение значений первых 12 бит является нечетным числом, контрольному биту присваивается значение 1, чтобы результат сложения 13 бит был четным.
        *  2-й контрольный бит:
        *       последние 13 бит всегда дают в сумме нечетное число.
        */

        void Init();

        void Transmit(CLRC66303HN::UID &);

        // Передать 26 бит, начиная со старшего
        void Transmit26bit(uint);

        // Передать один бит. По meter отмеряют время до старта передачи (1мс)
        void TransmitBit(bool, TimeMeterMS &meter);

        // Возвращает количество единиц в value от bit_start до bit_end
        int NumOnes(uint8 value, int bit_start, int bit_end);
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


void HAL_USART2_WG26::WG26::Transmit(CLRC66303HN::UID &uid)
{
    Mode::Transmit();

    uint8 bytes[3] = { uid.byte[2], uid.byte[1], uid.byte[0] };

    // Вычисляем бит чётности. Он должен быть таким, чтобы количество единиц в первых 13 битах было чётным

    int num_ones = NumOnes(bytes[0], 0, 7) + NumOnes(bytes[1], 4, 7);            // Количество единиц

    int bit_parity_start = (num_ones % 2) ? 1 : 0;

    // Вычисляем бит нечётности. Он должен быть таким, чтобы количество единиц в последних 13 битах было нечётным

    num_ones = NumOnes(bytes[1], 0, 3) + NumOnes(bytes[2], 0, 7);

    int bit_parity_end = (num_ones % 2) ? 0 : 1;

    uint value = (uint)(bit_parity_start << 31);        // Бит чётности первых 13 передаваемых бит
    value |= (bytes[0] << 23);                          // \ 
    value |= (bytes[1] << 15);                          // | Три байта от старшего к младшему
    value |= (bytes[2] << 7);                           // /
    value |= (bit_parity_end << 6);                     // Бит нечётности последних 13 передаваемых бит 

    Transmit26bit(value);

    Mode::Receive();
}


void HAL_USART2_WG26::WG26::Transmit26bit(uint value)
{
    D0::Hi();
    D1::Hi();

    TimeMeterMS meter;

    for (int i = 31; i >= 6; i--)
    {
        TransmitBit(value & (1 << i), meter);
    }

    D0::Hi();
    D1::Hi();

    meter.WaitFor(1);
}


void HAL_USART2_WG26::WG26::TransmitBit(bool bit, TimeMeterMS &meter)
{
    meter.WaitFor(1);

    meter.Reset();

    TimeMeterUS meterDuration;              // Для отмерения длительности импульса

    if (bit)
    {
        D1::Lo();
    }
    else
    {
        D0::Lo();
    }

    meterDuration.WaitFor(100);

    D0::Hi();
    D1::Hi();
}


int HAL_USART2_WG26::WG26::NumOnes(uint8 value, int bit_start, int bit_end)
{
    int result = 0;

    for (int i = bit_start; i <= bit_end; i++)
    {
        if (value & (1 << i))
        {
            result++;
        }
    }

    return result;
}


void HAL_USART2_WG26::WG26::Init()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_2 |                // A / D0 - без инверсии
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOA, &is);

    D0::Hi();
    D1::Hi();
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
