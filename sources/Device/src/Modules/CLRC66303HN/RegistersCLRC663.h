// 2022/7/6 9:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Register
{
    struct RegisterCLRC663
    {
        RegisterCLRC663(uint8 _address, int _data = 0) : address(_address), data(_data) { }
        void Write();
        void Write(uint8 data);
        uint8 Read();
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

        // clear - очистить буфер
        // waterLevelExtBit - 0 или 1 для 512-байтного FIFO
        void Write(Size::E, bool clear, int waterLevelExtBit);
    };


    struct FIFOData : public RegisterCLRC663
    {
        FIFOData() : RegisterCLRC663(0x05) { }

        void Write(uint8 data);

        void Write(uint8 data0, uint8 data1);

        void Read2Bytes(uint8 data[2]);
    };


    struct IRQ0 : public RegisterCLRC663
    {
        static const int Set = (1 << 7);
        static const int HiAlertIRQ = (1 << 6);
        static const int LoAlertIRQ = (1 << 5);
        static const int IdleIRQ = (1 << 4);
        static const int TxIRQ = (1 << 3);
        static const int RxIRQ = (1 << 2);
        static const int ErrIRQ = (1 << 1);
        static const int RxSOFIRQ = (1 << 0);

        IRQ0(int data = 0) : RegisterCLRC663(0x06, data) { }
    };
}

