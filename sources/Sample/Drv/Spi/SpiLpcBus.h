#pragma once


static const unsigned int LPC_SSP0_BASE = 1;

static const int LPC_SSP0 = 1;


struct LPC_SSP_TypeDef
{

};


class TSpiLpcBus
{
public:
    int swapBytes(LPC_SSP_TypeDef *, unsigned short, const unsigned char *, unsigned char *);
};


struct _LPC_GPIO2
{
    unsigned int MIS;
    unsigned int IC;
    unsigned int DIR;
    unsigned int IS;
    unsigned int IBE;
    unsigned int IEV;
    unsigned int IE;
    unsigned int DATA;
};


struct _LPC_IOCON
{
    unsigned int PIO0_1;
    unsigned int PIO3_1;
    unsigned int PIO2_2;
    unsigned int PIO0_2;
};


extern _LPC_GPIO2 *LPC_GPIO2;
extern _LPC_GPIO2 *LPC_GPIO0;
extern _LPC_IOCON *LPC_IOCON;
