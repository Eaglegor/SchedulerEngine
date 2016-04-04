#include "HyperbolaTemperatureScheduler.h"

namespace Scheduler
{
    HyperbolaTemperatureScheduler::HyperbolaTemperatureScheduler () :
        initial_value(50.f), minumum_value(0.1f)
    {
        reset();
    }

    HyperbolaTemperatureScheduler::HyperbolaTemperatureScheduler(float ainitial_value, float aminumum_value) :
        initial_value(ainitial_value), minumum_value(aminumum_value)
    {
        reset();
    }

    void HyperbolaTemperatureScheduler::changeTemperature()
    {
        current_value = initial_value / static_cast<float>(++schedule_time);
    }

    void HyperbolaTemperatureScheduler::reset()
    {
        current_value = initial_value;
        schedule_time = 0;
    }

    float HyperbolaTemperatureScheduler::getTemperature() const
    {
        return current_value;
    }

    bool HyperbolaTemperatureScheduler::isFinish() const
    {
        return current_value <= minumum_value;
    }

    float HyperbolaTemperatureScheduler::getInitialTemperature() const
    {
        return initial_value;
    }

    float HyperbolaTemperatureScheduler::getMinimalTemperature() const
    {
        return minumum_value;
    }

}
