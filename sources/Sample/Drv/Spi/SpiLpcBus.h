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
