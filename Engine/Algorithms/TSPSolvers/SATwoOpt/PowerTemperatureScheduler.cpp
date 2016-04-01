#include "PowerTemperatureScheduler.h"
#include <algorithm>

namespace Scheduler
{
    PowerTemperatureScheduler::PowerTemperatureScheduler () :
        initial_value(50.f), minumum_value(0.1f), change_speed(1.f - 0.05f)
    {
        reset();
    }

    PowerTemperatureScheduler::PowerTemperatureScheduler(float ainitial_value, float aminumum_value, float achange_speed) :
        initial_value(ainitial_value), minumum_value(aminumum_value), change_speed(achange_speed)
    {
        change_speed = std::max(std::numeric_limits<float>::epsilon(), change_speed);
        change_speed = std::min(1 - std::numeric_limits<float>::epsilon(), change_speed);

        reset();
    }

    void PowerTemperatureScheduler::changeTemperature()
    {
        current_value *= change_speed;
    }

    void PowerTemperatureScheduler::reset()
    {
        current_value = initial_value;
    }

    float PowerTemperatureScheduler::getTemperature() const
    {
        return current_value;
    }

    bool PowerTemperatureScheduler::isFinish() const
    {
        return current_value <= minumum_value;
    }

    float PowerTemperatureScheduler::getInitialTemperature() const
    {
        return initial_value;
    }

    float PowerTemperatureScheduler::getMinimalTemperature() const
    {
        return minumum_value;
    }
}
