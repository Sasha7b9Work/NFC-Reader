// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstring>
#include <cstdio>


namespace CLRC66303HN
{
    struct UID
    {
        UID()
        {
            Clear();
        }

        void Clear()
        {
            calculated = false;
            std::strcat(buffer, "null");
        }

        // Рассчиатать uid из 4 байт
        void Calculate4Bytes()
        {
            calculated = true;

            std::sprintf(buffer, "%02X:%02X:%02X:%02X", byte1, byte2, byte3, byte4);
        }

        char *ToString()
        {
            return buffer;
        }

        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;

        bool calculated;

        char buffer[30];
    };

    namespace Command
    {
        void Idle();

        void SendToCard(uint8);
    }

    // Команды работы с CLRC663
    namespace Command_
    {
        struct CommandCLRC663
        {
            CommandCLRC663(uint8 _command) : command(_command) { }

            void Run();

            uint8 command;
        };


        struct Transceive : public CommandCLRC663
        {
            Transceive() : CommandCLRC663(0x07) { }

            void Run();
            void Run(uint8 data);
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
