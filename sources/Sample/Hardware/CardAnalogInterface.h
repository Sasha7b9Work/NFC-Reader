#pragma once


class TCardAnalofInterface
{
public:
    void fieldOff();
    void fieldOn();
    int transceive(bool, const uint8_t *, int, uint8_t[36], size_t);
};
