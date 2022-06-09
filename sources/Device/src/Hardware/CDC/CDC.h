// 2022/04/20 08:54:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "usbd_cdc.h"


namespace CDC
{
    void Init();

    uint8 Transmit(char *format, ...);

    void OnIRQHandler();

    extern void *handlePCD;        // PCD_HandleTypeDef
};
