// 2022/7/3 11:16:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Modules/CLRC66303HN/CommandsCLRC663.h"


namespace CLRC66303HN
{
    void Init();

    void Update();

    uint8 GetRegister06();

    BitSet16 GetData();

    UID GetUID();

    char *Readed();
}
