// 2022/7/6 10:32:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/CLRC66303HN/CommandsCLRC663.h"
#include "Modules/CLRC66303HN/RegistersCLRC663.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <cstdio>


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


    bool Command::Card::AnticollisionCL(int cl, UID *uid)
    {
        Command::Idle();
        fifo.Clear();
        irq0.Clear();

        Register::RegisterCLRC663(0x2E).Write(0x08);        // All bits will be sent via NFC

        irq0.Clear();

        Command::Card::Send(cl == 1 ? 0x93 : 0x95, 0x20);

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
                    int i0 = 0;

                    if (cl == 2)
                    {
                        i0 += 5;
                    }

                    uid->byte[i0 + 0] = fifo.Pop();
                    uid->byte[i0 + 1] = fifo.Pop();
                    uid->byte[i0 + 2] = fifo.Pop();
                    uid->byte[i0 + 3] = fifo.Pop();
                    uid->byte[i0 + 4] = fifo.Pop();

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

        int i0 = 0;

        if (cl == 2)
        {
            i0 = 5;
        }

        Command::Card::Send(cl == 1 ? 0x93 : 0x95, 0x70,    uid->byte[i0 + 0],
                                                            uid->byte[i0 + 1],
                                                            uid->byte[i0 + 2],
                                                            uid->byte[i0 + 3],
                                                            uid->byte[i0 + 4]);

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
                        return true;
                    }

                    if (_GET_BIT(sak, 2) == 0)
                    {
                        uid->Calculate();
                        return true;
                    }
                }
            }
        }

        return false;
    }


    UID::UID()
    {
        Clear();
    }

    void UID::Clear()
    {
        std::memset(byte, 0, 10);
        std::memset(uid, 0, 7);

        std::strcpy(buffer, "null");

        calculated = false;
    }

    void UID::Calculate()
    {
        calculated = false;

        if (byte[0] != 0)
        {
            if (byte[0] == 0x88)
            {
                uid[0] = byte[1];
                uid[1] = byte[2];
                uid[2] = byte[3];
                uid[3] = byte[5];
                uid[4] = byte[6];
                uid[5] = byte[7];
                uid[6] = byte[8];
            }
            else
            {
                uid[0] = byte[0];
                uid[1] = byte[1];
                uid[2] = byte[2];
                uid[3] = byte[3];
            }

            calculated = true;
        }

        if (!calculated)
        {
            std::strcpy(buffer, "null");
        }
        else
        {
            if (byte[0] == 0x88)
            {
                std::sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X:%02X", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6]);
            }
            else
            {
                std::sprintf(buffer, "%02X:%02X:%02X:%02X", uid[0], uid[1], uid[2], uid[3]);
            }
        }
    }

    bool UID::Calcualted() const
    {
        return calculated;
    }
}
