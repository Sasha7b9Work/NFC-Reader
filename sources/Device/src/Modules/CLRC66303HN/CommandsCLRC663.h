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

            std::sprintf(buffer, "%02X:%02X:%02X:%02X", byte0, byte1, byte2, byte3);
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

        uint8 uid0;
        uint8 uid1;
        uint8 uid2;
        uint8 uid3;

        bool calculated;

        char buffer[30];
    };

    namespace Command
    {
        void Idle();

        namespace Card
        {
            void Send(uint8);

            void Send(uint8, uint8);

            void Send(uint8, uint8, uint8, uint8, uint8, uint8, uint8);

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
}
