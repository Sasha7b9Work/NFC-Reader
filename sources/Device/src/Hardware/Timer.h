// 2022/6/7 9:06:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


#define TIME_MS Timer::CurrentTime()


namespace Timer
{
    uint CurrentTime();

    void Delay(uint timeMS);
}



// ��������� ��� ������� �������
struct TimeMeterMS
{
    TimeMeterMS();

    // ���������� ������ �������
    void Reset();

    void Pause();

    void Continue();

    // ������� ����������� ������ � ������� ������ Reset()
    uint ElapsedTime();

private:

    uint time_reset;        // �� ����� ������� ������������� ElapsedTime()
    uint time_pause;        // � ���� ������ ��������� �� �����
};
