#include "BasicAcceptanceFunction.h"
#include <cstdlib>

namespace Scheduler
{
    BasicAcceptanceFunction::BasicAcceptanceFunction() :
        initial_temperature(2000.f), minimum_temperature(1.f), linear_temperature_change(20.f)
    {}

    BasicAcceptanceFunction::BasicAcceptanceFunction(float aInitialTemperature, float aMinimumTemperature, float aLinearTemperatureChange) :
        initial_temperature(aInitialTemperature), minimum_temperature(aMinimumTemperature), linear_temperature_change(aLinearTemperatureChange)
    {}

    bool BasicAcceptanceFunction::isAccepting(const Cost &delta, unsigned long long iterationNumber) const
    {
        const float temperature_value = calculateTemperature(iterationNumber);
        if (temperature_value <= minimum_temperature) {
            return false;
        }
        const float normalized_value = std::exp(-delta.getValue() / temperature_value);
        const int rand_value = std::rand();
        const float normalized_rand = static_cast<float>(rand_value) / static_cast<float>(RAND_MAX);

        return normalized_rand <= normalized_value;
    }

    float BasicAcceptanceFunction::calculateTemperature(unsigned long long iterationNumber) const
    {
        return initial_temperature - static_cast<float>(iterationNumber) * linear_temperature_change;
    }
}
