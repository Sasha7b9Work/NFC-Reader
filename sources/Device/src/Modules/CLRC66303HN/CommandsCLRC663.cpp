// 2022/7/6 10:32:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


namespace CLRC66303HN
{
    uint8 ReadRegister(uint8);

    void WriteRegister(uint8 reg, uint8 value);

    void Command::Idle()
    {
        WriteRegister(0x00, 0x00);
    }


    void Command::Card::Send(uint8 command)
    {
        fifo.Push(command);

        WriteRegister(0x00, 0x07);
    }


    void Command::Card::Send(uint8 command, uint8 data)
    {
        fifo.Push(command);
        fifo.Push(data);

        WriteRegister(0x00, 0x07);
    }


    void Command::Card::Send(uint8 command, uint8 data0, uint8 data1, uint8 data2, uint8 data3, uint8 data4, uint8 data5)
    {
        fifo.Push(command);
        fifo.Push(data0);
        fifo.Push(data1);
        fifo.Push(data2);
        fifo.Push(data3);
        fifo.Push(data4);
        fifo.Push(data5);

        WriteRegister(0x00, 0x07);
    }


    bool Command::Card::AnticollisionCL1(UID *uid)
    {
        Command::Idle();
        fifo.Clear();
        irq0.Clear();

        Register::RegisterCLRC663(0x2E).Write(0x08);        // All bits will be sent via NFC

        irq0.Clear();

        Command::Card::Send(0x93, 0x20);

        TimeMeterMS meter;

        while (meter.ElapsedTime() < 10)
        {
            if (irq0.GetValue() & IRQ0::RxIRQ)
            {
                if (Register::Error().Read() & Register::Error::CollDet)
                {
                    return false;
                }

                if (irq0.GetValue() & IRQ0::ErrIRQ)
                {
                    return false;
                }
                else
                {
                    uid->byte0 = fifo.Pop();
                    uid->byte1 = fifo.Pop();
                    uid->byte2 = fifo.Pop();
                    uid->byte3 = fifo.Pop();
                    uid->byte4 = fifo.Pop();

                    return true;
                }
            }
        }

        return false;
    }


    bool Command::Card::SelectCL(int cl, UID *uid)
    {
        Command::Idle();
        fifo.Clear();

        Register::RegisterCLRC663(0x2C).Write(0x19);        // Switches the CRC extention ON in tx direction
        Register::RegisterCLRC663(0x2D).Write(0x19);        // Switches the CRC extention OFF in rx direction

        Register::RegisterCLRC663(0x2E).Write(0x08);

        irq0.Clear();

        Command::Card::Send(0x93, 0x70, uid->byte0, uid->byte1, uid->byte2, uid->byte3, uid->byte4);

        TimeMeterMS meter;

        while (meter.ElapsedTime() < 10)
        {
            if (irq0.GetValue() & IRQ0::RxIRQ)
            {
                if (irq0.GetValue() & IRQ0::ErrIRQ)
                {
                    return false;
                }
                else
                {
                    uint8 sak = fifo.Pop();

                    if (_GET_BIT(sak, 2) == 1)
                    {
                        std::strcat(uid->buffer, "not complete");
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


    bool Command::Card::AnticollisionCL2(UID *)
    {
        return false;
    }
}
