#pragma once


static const int TICKS_PER_SEC = 50;


namespace OS
{
    void sleep(int);

    struct scmRTOS_ISRW_TYPE
    {
    };

    struct TEventFlag
    {
        static void signal();
        bool wait(int);
    };
}
