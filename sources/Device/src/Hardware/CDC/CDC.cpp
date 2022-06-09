// 2022/04/20 08:53:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Hardware/CDC/CDC.h"
#include "Hardware/HAL/HAL.h"
#include <usbd_desc.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>


static USBD_HandleTypeDef hUsbDeviceFS;
static PCD_HandleTypeDef _handlePCD;

void *CDC::handlePCD = &_handlePCD;


#define APP_RX_DATA_SIZE  1000
#define APP_TX_DATA_SIZE  1000

// Create buffer for reception and transmission
// It's up to user to redefine and/or remove those define
// Received data over USB are stored in this buffer
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

// Data to send over USB CDC are stored in this buffer
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

extern USBD_HandleTypeDef hUsbDeviceFS;

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
    CDC_Init_FS,
    CDC_DeInit_FS,
    CDC_Control_FS,
    CDC_Receive_FS
};


namespace CDC
{
    uint8 Transmit(const void *buffer, int size);
}


void CDC::Init()
{
    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    USBD_Start(&hUsbDeviceFS);
}


static int8_t CDC_Init_FS(void)
{
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
    return (USBD_OK);
}


static int8_t CDC_DeInit_FS(void)
{
    return (USBD_OK);
}


static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* /*pbuf*/, uint16_t /*length*/)
{
    switch(cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
        break;

    case CDC_SET_COMM_FEATURE:
        break;

    case CDC_GET_COMM_FEATURE:
        break;

    case CDC_CLEAR_COMM_FEATURE:
        break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
        break;

    case CDC_GET_LINE_CODING:
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        break;

    case CDC_SEND_BREAK:
        break;

    default:
        break;
  }

  return (USBD_OK);
}


static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t */*Len*/)
{
    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
}


#ifndef GUI

uint8_t CDC::Transmit(const void *buffer, int size)
{
    if (!buffer)
    {
        return USBD_OK;
    }

    uint8_t result = USBD_OK;

    USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;

    if (hcdc->TxState != 0)
    {
        return USBD_BUSY;
    }

    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t *)buffer, (uint16_t)size);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);

    return result;
}

#endif


void CDC::OnIRQHandler()
{
    HAL_PCD_IRQHandler(&_handlePCD);
}


uint8 CDC::Transmit(char *format, ...)
{
    char buffer[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(buffer, format, args);
    va_end(args);

    return Transmit((const void *)buffer, (int)std::strlen(buffer));
}
