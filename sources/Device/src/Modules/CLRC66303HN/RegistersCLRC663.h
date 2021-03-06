// 2022/7/6 9:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CLRC66303HN
{
    struct FIFO
    {
        void Init();

        void Clear();

        void Push(uint8);

        uint8 Pop();
    };

    struct IRQ0
    {
        static const int Set = (1 << 7);
        static const int HiAlertIRQ = (1 << 6);
        static const int LoAlertIRQ = (1 << 5);
        static const int IdleIRQ = (1 << 4);
        static const int TxIRQ = (1 << 3);
        static const int RxIRQ = (1 << 2);
        static const int ErrIRQ = (1 << 1);
        static const int RxSOFIRQ = (1 << 0);

        void Clear();

        uint8 GetValue();

        // ?????????? true, ???? "??????????" ????? IRQ ????????????? ? ?????????? ??????
        bool DataReadyHardware();
    };

    extern FIFO fifo;
    extern IRQ0 irq0;


    namespace Register
    {
        struct RegisterCLRC663
        {
            RegisterCLRC663(uint8 _address, int _data = 0) : address(_address), data(_data) { }
            void Write();
            void Write(uint8 data);
            void Write(uint8 data1, uint8 data2);
            uint8 Read();
            void Read(uint8 out[2], uint8 in[2]);
            uint8 address;
            int data;
        };


        struct Error : public RegisterCLRC663
        {
            static const int CollDet = (1 << 2);

            Error() : RegisterCLRC663(0x0A) { }
        };
    }

}
