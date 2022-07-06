// 2022/7/6 9:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Register
{
    struct RegisterCLRC663
    {
        RegisterCLRC663(uint8 _address) : address(_address) { }
        uint8 address;
    };


    struct FIFOControl : public RegisterCLRC663
    {
        struct Size
        {
            // It is recommended to change the FIFO size only, when the FIFO content had been cleared.
            enum E
            {
                _512,
                _256
            };
        };

        FIFOControl() : RegisterCLRC663(0x02) { }

        // clear - очистить буфер
        // waterLevelExtBit - 0 или 1 для 512-байтного FIFO
        void Write(Size::E, bool clear, int waterLevelExtBit);
    };


    struct FIFOData : public RegisterCLRC663
    {
        FIFOData() : RegisterCLRC663(0x05) { }

        void Write(uint8 data0, uint8 data1);
    };


    struct DrvMode : public RegisterCLRC663
    {
        DrvMode() : RegisterCLRC663(0x28) { }

        void Write(bool Tx2Inv, bool Tx1Inv, bool TxEn, uint8 TxClkMode);
    };
}

