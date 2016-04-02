#include "LinearTemperatureScheduler.h"

namespace Scheduler
{
    LinearTemperatureScheduler::LinearTemperatureScheduler () :
        initial_value(50.f), minumum_value(0.1f), change_speed(0.05f)
    {
        reset();
    }

    LinearTemperatureScheduler::LinearTemperatureScheduler(float ainitial_value, float aminumum_value, float achange_speed) :
        initial_value(ainitial_value), minumum_value(aminumum_value), change_speed(achange_speed)
    {
        reset();
    }

    void LinearTemperatureScheduler::changeTemperature()
    {
        current_value -= change_speed;
    }

    void LinearTemperatureScheduler::reset()
    {
        current_value = initial_value;
    }

    float LinearTemperatureScheduler::getTemperature() const
    {
        return current_value;
    }

    bool LinearTemperatureScheduler::isFinish() const
    {
        return current_value <= minumum_value;
    }

    float LinearTemperatureScheduler::getInitialTemperature() const
    {
        return initial_value;
    }

    float LinearTemperatureScheduler::getMinimalTemperature() const
    {
        return minumum_value;
    }
}
