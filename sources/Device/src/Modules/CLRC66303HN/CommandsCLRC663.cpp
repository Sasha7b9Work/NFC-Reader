// 2022/7/6 10:32:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"


void Command::CommandCLRC663::Run()
{
    uint8 buffer[2] = { 0x00, command };

    HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
}


void Command::LoadProtocol::Run(uint8 protocol_rx, uint8 protocol_tx)
{
    Register::FIFOData().Write(protocol_rx, protocol_tx);

    CommandCLRC663::Run();
}
