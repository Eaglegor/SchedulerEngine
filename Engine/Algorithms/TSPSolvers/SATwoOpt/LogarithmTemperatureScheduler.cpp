#include "LogarithmTemperatureScheduler.h"
#include <cmath>

namespace Scheduler
{
    LogarithmTemperatureScheduler::LogarithmTemperatureScheduler () :
        initial_value(20.f), minumum_value(2.f)
    {
        reset();
    }

    LogarithmTemperatureScheduler::LogarithmTemperatureScheduler(float ainitial_value, float aminumum_value) :
        initial_value(ainitial_value), minumum_value(aminumum_value)
    {
        reset();
    }

    void LogarithmTemperatureScheduler::changeTemperature()
    {
        current_value = initial_value/ std::log(++schedule_time);
    }

    void LogarithmTemperatureScheduler::reset()
    {
        current_value = initial_value;
        schedule_time = 0;
    }

    float LogarithmTemperatureScheduler::getTemperature() const
    {
        return current_value;
    }

    bool LogarithmTemperatureScheduler::isFinish() const
    {
        return current_value <= minumum_value;
    }

    float LogarithmTemperatureScheduler::getInitialTemperature() const
    {
        return initial_value;
    }

    float LogarithmTemperatureScheduler::getMinimalTemperature() const
    {
        return minumum_value;
    }
}
