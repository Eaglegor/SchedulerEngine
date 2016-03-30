#include "LinearTemperatureFunction.h"

namespace Scheduler
{
    LinearTemperatureFunction::LinearTemperatureFunction () :
        initial_value(50.f), minumum_value(0.1f), change_speed(0.05f)
    {
        reset();
    }

    LinearTemperatureFunction::LinearTemperatureFunction(float ainitial_value, float aminumum_value, float achange_speed) :
        initial_value(ainitial_value), minumum_value(aminumum_value), change_speed(achange_speed)
    {
        reset();
    }

    void LinearTemperatureFunction::changeTemperature()
    {
        current_value -= change_speed;
    }

    void LinearTemperatureFunction::reset()
    {
        current_value = initial_value;
    }

    float LinearTemperatureFunction::getTemperature() const
    {
        return current_value;
    }

    bool LinearTemperatureFunction::isFinish() const
    {
        return current_value <= minumum_value;
    }
}
