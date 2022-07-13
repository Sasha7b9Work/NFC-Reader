// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CLRC66303HN
{
    namespace Command
    {
        struct CommandCLRC663
        {
            CommandCLRC663(uint8 _command) : command(_command) { }

            void Run();

            uint8 command;
        };


        struct Idle : public CommandCLRC663
        {
            Idle() : CommandCLRC663(0x00) { }
        };


        struct Transceive : public CommandCLRC663
        {
            Transceive() : CommandCLRC663(0x07) { }

            void Run(uint8 data);
        };


        struct LoadProtocol : public CommandCLRC663
        {
            LoadProtocol() : CommandCLRC663(0x0D) { }

            void Run(uint8 protocol_rx, uint8 protocol_tx);
        };
    }
}
