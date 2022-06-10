// 2022/6/10 9:08:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/W25Q80DV/W25Q80DV.h"



void W25Q80DV::Write(void *buffer, int size)
{
    /*
    *   20h   Sector erase   36
    *   06h   Write Enable   22
    *   02h   Page Program   34
    *   04h   Write Disable  23
    */


}


void W25Q80DV::Read(void *buffer, int size)
{

}
