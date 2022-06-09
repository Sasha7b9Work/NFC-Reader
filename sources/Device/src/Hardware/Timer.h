// 2022/6/7 9:06:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


#define TIME_MS Timer::CurrentTime()


namespace Timer
{
    uint CurrentTime();

    void Delay(uint timeMS);
}



// Структура для отсчёта времени
struct TimeMeterMS
{
    TimeMeterMS();

    // Установить момент отсчёта
    void Reset();

    void Pause();

    void Continue();

    // Столько миллисекунд прошло с момента вызова Reset()
    uint ElapsedTime();

private:

    uint time_reset;        // От этого времени отсчитывается ElapsedTime()
    uint time_pause;        // В этот момент поставили на паузу
};
