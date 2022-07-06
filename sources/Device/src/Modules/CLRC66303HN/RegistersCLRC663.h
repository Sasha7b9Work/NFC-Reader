// 2022/7/6 9:44:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Register
{
    struct RegisterCLRC663
    {

 
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

        // clear - очистить буфер
        // waterLevelExtBit - 0 или 1 для 512-байтного FIFO
        void Write(Size::E, bool clear, int waterLevelExtBit);
    };


    struct FIFOData : public RegisterCLRC663
    {
        void Write(uint8 data0, uint8 data1);
    };
}

