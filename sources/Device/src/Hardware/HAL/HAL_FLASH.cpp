// 2022/7/15 9:04:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include <stm32f1xx_hal.h>


namespace HAL_FLASH
{
    static uint addressAntenna  = 0x8000000 + 0x0FF28;
    static uint addressProtocol = 0x8000000 + 0x0FF48;

    // Возвращает true, если конфигурация антенны уже есть в памяти
    static bool ConfigExistAntenna();
    static bool ConfigExistProtocol();

    // Сохранить конфиг в память
    static void SaveConfigAntenna();
    static void SaveConfigProtocol();
}


void HAL_FLASH::LoadAntennaConfiguration106()
{
    if (!ConfigExistAntenna())
    {
        SaveConfigAntenna();
    }

    uint8 *address = (uint8 *)addressAntenna;

    uint8 reg = 0x28;

    while (reg <= 0x39)
    {
        CLRC66303HN::Register::RegisterCLRC663(reg).Write(*address);
        reg++;
        address++;
    }
}


void HAL_FLASH::LoadProtocol()
{
    if (!ConfigExistProtocol())
    {
        SaveConfigProtocol();
    }

    uint8 *address = (uint8 *)addressProtocol;

    uint8 reg = 0x48;

    while (reg <= 0x5F)
    {
        CLRC66303HN::Register::RegisterCLRC663(reg).Write(*address);
        reg++;
        address++;
    }
}


bool HAL_FLASH::ConfigExistAntenna()
{
    uint8 *address = (uint8 *)addressAntenna;

    return (*address) != 0xFF;
}


bool HAL_FLASH::ConfigExistProtocol()
{
    uint8 *address = (uint8 *)addressProtocol;

    return (*address) != 0xFF;
}


void HAL_FLASH::SaveConfigAntenna()
{
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

    uint address = addressAntenna;

    uint8 reg = 0x28;

    while (reg <= 0x39)
    {
        uint8 reg_lo = CLRC66303HN::Register::RegisterCLRC663(reg).Read();
        uint8 reg_hi = CLRC66303HN::Register::RegisterCLRC663((uint8)(reg + 1)).Read();

        uint16 value = (uint16)((reg_hi << 8) | reg_lo);

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, value);

        reg += 2;
        address += 2;
    }

    HAL_FLASH_Lock();
}


void HAL_FLASH::SaveConfigProtocol()
{
    CLRC66303HN::Register::RegisterCLRC663(0x48).Write(0x20);    // TxBitMod     20
    CLRC66303HN::Register::RegisterCLRC663(0x49).Write(0x00);    // RFU          00
    CLRC66303HN::Register::RegisterCLRC663(0x4A).Write(0x04);    // TxDataCon    04
    CLRC66303HN::Register::RegisterCLRC663(0x4B).Write(0x50);    // TxDataMod    50
    CLRC66303HN::Register::RegisterCLRC663(0x4C).Write(0x40);    // TxSymFreq    40
    CLRC66303HN::Register::RegisterCLRC663(0x4D).Write(0x00);    // TxSym0H      00
    CLRC66303HN::Register::RegisterCLRC663(0x4E).Write(0x00);    // TxSym0L      00
    CLRC66303HN::Register::RegisterCLRC663(0x4F).Write(0x00);    // TxSym1H      00
    CLRC66303HN::Register::RegisterCLRC663(0x50).Write(0x00);    // TxSym1L      00
    CLRC66303HN::Register::RegisterCLRC663(0x51).Write(0x00);    // TxSym2       00
    CLRC66303HN::Register::RegisterCLRC663(0x52).Write(0x00);    // TxSym3       00
    CLRC66303HN::Register::RegisterCLRC663(0x53).Write(0x00);    // TxSym10Len   00
    CLRC66303HN::Register::RegisterCLRC663(0x54).Write(0x00);    // TxSym32Len   00
    CLRC66303HN::Register::RegisterCLRC663(0x55).Write(0x00);    // TxSym10BurstCtrl 00
    CLRC66303HN::Register::RegisterCLRC663(0x56).Write(0x00);    // TxSym10Mod   00
    CLRC66303HN::Register::RegisterCLRC663(0x57).Write(0x50);    // TxSym32Mod   50
    CLRC66303HN::Register::RegisterCLRC663(0x58).Write(0x02);    // RxBitMod     02
    CLRC66303HN::Register::RegisterCLRC663(0x59).Write(0x00);    // RxEofSym     00
    CLRC66303HN::Register::RegisterCLRC663(0x5A).Write(0x00);    // RxSyncValH   00
    CLRC66303HN::Register::RegisterCLRC663(0x5B).Write(0x01);    // RxSyncVaIL   01
    CLRC66303HN::Register::RegisterCLRC663(0x5C).Write(0x00);    // RxSyncMod    00
    CLRC66303HN::Register::RegisterCLRC663(0x5D).Write(0x08);    // RxMod        08
    CLRC66303HN::Register::RegisterCLRC663(0x5E).Write(0x80);    // RxCorr       80
    CLRC66303HN::Register::RegisterCLRC663(0x5F).Write(0xB2);    // FabCal       B2

    HAL_FLASH_Unlock();

    uint address = addressProtocol;

    uint8 reg = 0x48;

    while (reg <= 0x5F)
    {
        uint8 reg_lo = CLRC66303HN::Register::RegisterCLRC663(reg).Read();
        uint8 reg_hi = CLRC66303HN::Register::RegisterCLRC663((uint8)(reg + 1)).Read();

        uint16 value = (uint16)((reg_hi << 8) | reg_lo);

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, value);

        reg += 2;
        address += 2;
    }
}
