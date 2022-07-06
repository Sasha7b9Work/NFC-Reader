// 2022/7/6 9:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Register
{
    struct RegisterCLRC663
    {
        RegisterCLRC663(uint8 _address, int _data = 0) : address(_address), data(_data) { }
        void Write();
        uint8 address;
        int data;
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

        // clear - �������� �����
        // waterLevelExtBit - 0 ��� 1 ��� 512-�������� FIFO
        void Write(Size::E, bool clear, int waterLevelExtBit);
    };


    struct FIFOData : public RegisterCLRC663
    {
        FIFOData() : RegisterCLRC663(0x05) { }

        void Write(uint8 data0, uint8 data1);
    };


    struct IRQ0 : public RegisterCLRC663
    {
        IRQ0(int data) : RegisterCLRC663(0x06, data) { }
    };


    struct DrvMode : public RegisterCLRC663
    {
        DrvMode() : RegisterCLRC663(0x28) { }

        void Write(bool Tx2Inv, bool Tx1Inv, bool TxEn, uint8 TxClkMode);
    };


    struct TxCrcPreset : public RegisterCLRC663
    {
        TxCrcPreset(int data) : RegisterCLRC663(0x2C, data) { }
    };
}

