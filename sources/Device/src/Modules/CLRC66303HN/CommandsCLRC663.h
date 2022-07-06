// 2022/7/6 10:32:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Command
{
    struct CommandCLRC663
    {
        void Run();
    };

    struct Idle : public CommandCLRC663
    {

    };
}

