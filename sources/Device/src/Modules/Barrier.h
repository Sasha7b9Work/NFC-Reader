// 2022/6/7 12:28:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Barrier
{
    void Init();

    // Переключить из закрытого в открытое состояние или обратно
    void Switch();

    void Open();

    bool IsOpened();

    // Прошло времени после последнего переключения
    uint TimeElapsed();
}
