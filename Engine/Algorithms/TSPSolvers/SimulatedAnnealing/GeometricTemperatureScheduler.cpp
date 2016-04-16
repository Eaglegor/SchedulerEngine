#include "GeometricTemperatureScheduler.h"
#include <algorithm>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
    GeometricTemperatureScheduler::GeometricTemperatureScheduler () :
        p0(-2.3f), p1(-690.f), iterations_count(1000)
    {
        initialize(100.f);
    }

    GeometricTemperatureScheduler::GeometricTemperatureScheduler(float ap0, float ap1, size_t aiterations_count) :
        p0(ap0), p1(ap1), iterations_count(aiterations_count)
    {
        initialize(100.f);
    }

    void GeometricTemperatureScheduler::changeTemperature()
    {
        current_value *= change_speed;
    }

    void GeometricTemperatureScheduler::adapt(float value)
    {}

    void GeometricTemperatureScheduler::initialize(Run* run, ScheduleCostFunction* schedule_cost_function, unsigned long long seed_value)
    {
        const auto costs = create_initial_costs(run, schedule_cost_function, 1000, seed_value);

        const auto best_cost_it = std::min_element(costs.begin(), costs.end());
        const auto best_cost = *best_cost_it;
        float sum_diff = 0.f;
        for (const auto &cost : costs) {
            sum_diff += (cost - best_cost).getValue();
        }

        initialize(sum_diff / costs.size());
    }

    void GeometricTemperatureScheduler::initialize(float avgCost)
    {
        initial_value = -avgCost / p0;
        minumum_value = -avgCost / p1;
        change_speed = std::pow(minumum_value / initial_value, 1.f / iterations_count);
        current_value = initial_value;
    }

    float GeometricTemperatureScheduler::getTemperature() const
    {
        return current_value;
    }

    bool GeometricTemperatureScheduler::isFinish() const
    {
        return current_value <= minumum_value;
    }
}
