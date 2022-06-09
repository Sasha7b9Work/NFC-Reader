// 2022/05/04 11:34:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"


namespace Math
{
    uint SDBMHash(uint hash, uint8 d)
    {
        return d + (hash << 6) + (hash << 16) - hash;
    }
}


uint Math::CalculateHash(const void *buffer, int size)
{
    uint hash = 0;

    const uint8 *data = (const uint8 *)buffer;

    while (size--)
    {
        hash = SDBMHash(hash, *data++);
    }

    return hash;
}
