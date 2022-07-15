// 2022/7/15 9:04:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include <stm32f1xx_hal.h>


namespace HAL_FLASH
{
    static uint start_address = 0x8000000 + 0x0FF28;

    // Возвращает true, если конфигурация антенны уже есть в памяти
    bool ConfigExist();

    // Сохранить конфиг в память
    void SaveConfig();
}


void HAL_FLASH::LoadAntennaConfiguration106()
{
    if (!ConfigExist())
    {
        SaveConfig();
    }

    uint8 *address = (uint8 *)start_address;

    uint8 reg = 0x28;

    while (reg <= 0x39)
    {
        CLRC66303HN::Register::RegisterCLRC663(reg).Write(*address);
        reg++;
        address++;
    }
}


bool HAL_FLASH::ConfigExist()
{
    uint8 *address = (uint8 *)start_address;

    return (*address) != 0xFF;
}


void HAL_FLASH::SaveConfig()
{
    /*
    CLRC66303HN::Register::RegisterCLRC663(0x28).Write(0x86);    // DrvMode
    CLRC66303HN::Register::RegisterCLRC663(0x29).Write(0x1F);    // TxAmp
    CLRC66303HN::Register::RegisterCLRC663(0x2A).Write(0x39);    // DrvCon
    CLRC66303HN::Register::RegisterCLRC663(0x2B).Write(0x0A);    // Txl
    CLRC66303HN::Register::RegisterCLRC663(0x2C).Write(0x18);    // TXCrcPreset
    CLRC66303HN::Register::RegisterCLRC663(0x2D).Write(0x18);    // RxCrcCon
    CLRC66303HN::Register::RegisterCLRC663(0x2E).Write(0x0F);    // TxDataNum
    CLRC66303HN::Register::RegisterCLRC663(0x2F).Write(0x21);    // TxModWidth
    CLRC66303HN::Register::RegisterCLRC663(0x30).Write(0x00);    // TxSym10BurstLen
    CLRC66303HN::Register::RegisterCLRC663(0x31).Write(0xC0);    // TxWaitCtrl
    CLRC66303HN::Register::RegisterCLRC663(0x32).Write(0x12);    // TxWaitLo
    CLRC66303HN::Register::RegisterCLRC663(0x33).Write(0xCF);    // TxFrameCon
    CLRC66303HN::Register::RegisterCLRC663(0x34).Write(0x00);    // RsSofD
    CLRC66303HN::Register::RegisterCLRC663(0x35).Write(0x04);    // RxCtrl
    CLRC66303HN::Register::RegisterCLRC663(0x36).Write(0x90);    // RxWait
    CLRC66303HN::Register::RegisterCLRC663(0x37).Write(0x5C);    // RxThreshold
    CLRC66303HN::Register::RegisterCLRC663(0x38).Write(0x12);    // Rcv
    CLRC66303HN::Register::RegisterCLRC663(0x39).Write(0x0A);    // RxAna

    HAL_FLASH_Unlock();

    uint address = start_address;

    uint8 reg = 0x28;

    while (reg <= 0x39)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD)
    }

    HAL_FLASH_Lock();
    */

    /*
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef is;
    is.TypeErase = FLASH_TYPEERASE_PAGES;
    is.PageAddress = 0x0800FC00;
    is.NbPages = 1;

    HAL_FLASHEx_Erase(&is)
    */
}
