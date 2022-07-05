// 2022/6/10 9:03:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace W25Q80DV
{
    // ������ ������ ��������� 1024 �������
    void Write1024bytes(const uint8 *buffer, int size);

    // ������ ������ ��������� 1024 �������
    void Read1024bytes(uint8 *buffer, int size);

    void ReadID();

    bool Test();

    // ���������� ��������, ��������� �� ����� �����
    uint8 TestValue();
}
