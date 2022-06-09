// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/CDC/CDC.h"
#include "Hardware/Timer.h"
#include "Modules/ADC.h"
#include "Modules/Barrier.h"
#include "Processor.h"


void Device::Init()
{
    HAL::Init();

    Barrier::Init();

    Barrier::Open();

    Timer::Delay(500);

    ADC::Init();
}


void Device::Update()
{
    if (Barrier::TimeElapsed() >= 200)
    {
//        Barrier::Switch();
    }

    if (ADC::DataReady())
    {
        float voltage = ADC::GetVoltage();

        CDC::Transmit("%f", voltage);

        Processor::AppendData(voltage, Barrier::IsOpened());

        Processor::Log();
    }
    else
    {
        CDC::Transmit("Not data");
    }
}
