#pragma once


static const unsigned int LPC_SSP0_BASE = 1;

static const int LPC_SSP0 = 1;


struct LPC_SSP_TypeDef
{

};


class TSpiLpcBus
{
public:
    int swapBytes(LPC_SSP_TypeDef *, uint16_t, const uint8_t *, uint8_t *);
};


struct _LPC_GPIO2
{
    uint32_t MIS;
    uint32_t IC;
    uint32_t DIR;
};


struct _LPC_IOCON
{
    uint32_t PIO3_1;
};


extern _LPC_GPIO2 *LPC_GPIO2;
extern _LPC_IOCON *LPC_IOCON;
