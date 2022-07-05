#pragma once


class TSpiAbstractBus
{
public:
    bool lock();
    bool swap(unsigned char, const unsigned char *, unsigned char *);
    bool unlock();
};
