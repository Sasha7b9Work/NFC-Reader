// 2022/6/19 6:30:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Beeper
{
    void Init();

    // Если wait == true, то ожидать завершения звука
    void Beep(int frequency, int timeMS, bool wait = false);
}
