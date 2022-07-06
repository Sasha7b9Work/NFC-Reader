#pragma once


class TCardAnalofInterface
{
public:
    void fieldOff();
    void fieldOn();
    int transceive(bool crc_enable, const uint8_t *fifo, int num_bits_fifo, uint8_t *rx_buf, size_t size_buf);
};
