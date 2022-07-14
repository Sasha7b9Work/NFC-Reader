// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CLRC66303HN
{
    struct UID
    {
        UID();

        void Clear();

        void Calculate();

        char *ToString() { return buffer; };

        bool Calcualted() const;

        // ѕервые 5 байт - 1 каскад, вторые 5 байт - второй каскад
        uint8 byte[10];

        uint8 uid[7];

    private:

        bool calculated;

        char buffer[50];
    };

    namespace Command
    {
        void Idle();

        namespace Card
        {
            void Send(uint8);

            void Send(uint8, uint8);

            void Send(uint8, uint8, uint8, uint8, uint8, uint8, uint8);

            // ѕосылают команду антиколлизии CL1 и ожидает ответ. ¬озвращает true в случае получени€ ответа
            bool AnticollisionCL(int cl, UID *uid);

            bool SelectCL(int cl, UID *uid);
        }
    }
}
