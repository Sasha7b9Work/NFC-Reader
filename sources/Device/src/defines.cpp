// 2022/7/13 9:46:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include <cstdio>


GlobalFlags gf;


void GlobalFlags::Clear()
{
    num_result = 0;
}


char *GlobalFlags::ToString()
{
    std::sprintf(buffer, "num_result:%d", num_result);

    return buffer;
}


