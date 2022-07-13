// 2022/7/6 10:32:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


namespace CLRC66303HN
{
    void Command::CommandCLRC663::Run()
    {
        uint8 buffer[2] = { 0x00, command };

        HAL_SPI::Write(DirectionSPI::Reader, buffer, 2);
    }


    void Command::Transceive::Run(uint8 data)
    {
        Register::FIFOData().Write(data);

        CommandCLRC663::Run();
    }


    void Command::Transceive::Run()
    {
        CommandCLRC663::Run();
    }


    void Command::LoadProtocol::Run(uint8 protocol_rx, uint8 protocol_tx)
    {
        Register::FIFOData().Write(protocol_rx, protocol_tx);

        CommandCLRC663::Run();
    }


    bool Request::AnticollisionCL1::Transceive(UID *uid)
    {
        Register::IRQ0 irq0;
        Register::FIFOData fifo;

        Command::Idle().Run();
        Register::FIFOControl().Clear256();
        irq0.Clear();

        Register::RegisterCLRC663(0x2E).Write(0x08);        // All bits will be sent via NFC

        fifo.Write(0x93);
        fifo.Write(0x20);

        irq0.Clear();

        Command::Transceive().Run();

        TimeMeterMS meter;

        while (meter.ElapsedTime() < 10)
        {
            if (irq0.Read() & Register::IRQ0::RxIRQ)
            {
                if (Register::Error().Read() & Register::Error::CollDet)
                {
                    return false;
                }

                if (irq0.Read() & Register::IRQ0::ErrIRQ)
                {
                    return false;
                }
                else
                {
                    uid->byte0 = fifo.Read();
                    uid->byte1 = fifo.Read();
                    uid->byte2 = fifo.Read();
                    uid->byte3 = fifo.Read();
                    uid->byte4 = fifo.Read();

                    return true;
                }
            }
        }

        return false;
    }


    bool Request::SelectCL1::Transceive(UID *uid)
    {
        Register::IRQ0 irq0;
        Register::FIFOData fifo;

        Command::Idle().Run();
        Register::FIFOControl().Clear256();

        Register::RegisterCLRC663(0x2C).Write(0x19);        // Switches the CRC extention ON in tx direction
        Register::RegisterCLRC663(0x2D).Write(0x19);        // Switches the CRC extention OFF in rx direction

        irq0.Clear();

        Register::RegisterCLRC663(0x2E).Write(0x08);

        fifo.Write(0x93);
        fifo.Write(0x70);
        fifo.Write(uid->byte0);
        fifo.Write(uid->byte1);
        fifo.Write(uid->byte2);
        fifo.Write(uid->byte3);
        fifo.Write(uid->byte4);

        irq0.Clear();

        Command::Transceive().Run();

        TimeMeterMS meter;

        while (meter.ElapsedTime() < 10)
        {
            if (irq0.Read() & Register::IRQ0::RxIRQ)
            {
                if (irq0.Read() & Register::IRQ0::ErrIRQ)
                {
                    return false;
                }
                else
                {
                    uint8 sak = fifo.Read();

                    if (_GET_BIT(sak, 2) == 1)
                    {
                        uid->Clear();
                        return true;
                    }

                    if (_GET_BIT(sak, 2) == 0)
                    {
                        uid->Calculate4Bytes();
                        return true;
                    }
                }
            }
        }

        return false;
    }


    bool Request::AnticollisionCL2::Transceive(UID *)
    {
        return false;
    }


    bool Request::SelectCL2::Transceive(UID *)
    {
        return false;
    }
}
