// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CLRC66303HN
{
    struct UID
    {
        UID() : byte0(0), byte1(1), byte2(2), byte3(3), byte4(4) {}

        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
    };

    // Команды работы с CLRC663
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

            void Run();
            void Run(uint8 data);
        };


        struct LoadProtocol : public CommandCLRC663
        {
            LoadProtocol() : CommandCLRC663(0x0D) { }

            void Run(uint8 protocol_rx, uint8 protocol_tx);
        };
    }

    // Команды работы с картой
    namespace Request
    {
        // Посылают команду антиколлизии CL1 и ожидает ответ. Возвращает true в случае получения ответа
        struct AnticollisionCL1
        {
            bool Transceive(UID *uid);
        };

        struct SelectCL1
        {
            bool Transceive(UID *uid);
        };

        struct AnticollisionCL2
        {
            bool Transceive(UID *uid);
        };

        struct SelectCL2
        {
            bool Transceive(UID *uid);
        };
    }
}
