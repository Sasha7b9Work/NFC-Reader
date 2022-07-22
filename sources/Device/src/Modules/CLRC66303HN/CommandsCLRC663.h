// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CLRC66303HN
{
    struct UID
    {
        UID();

        void Clear();

        // ��������� UID ������ �� �������� � ����� ������ byte[0...9]
        void Calculate();

        char *ToString() { return buffer; };

        // ���������� true, ���� UID ���������
        bool Calcualted() const;

        // ������ 5 ���� - 1 ������, ������ 5 ���� - ������ ������
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

            // �������� ������� ������������ CL1 � ������� �����. ���������� true � ������ ��������� ������
            bool AnticollisionCL(int cl, UID *uid);

            bool SelectCL(int cl, UID *uid);
        }
    }
}
