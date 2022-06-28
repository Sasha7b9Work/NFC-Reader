// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint;
typedef const char *pchar;
typedef unsigned char uchar;


#ifndef WIN32
    #define nullptr 0
    #pragma diag_suppress 2748
#endif


#define IN_MODE_TEST


union BitSet32
{
    uint   word;
    uint16 half_word[2];
    uint8  byte[4];
};


#define _GET_BIT(value, bit)   (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)   ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << (bit))))


#define ERROR_VALUE_FLOAT   1.111e29f
