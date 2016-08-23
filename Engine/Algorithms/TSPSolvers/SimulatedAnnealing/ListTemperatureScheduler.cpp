#include "ListTemperatureScheduler.h"
#include <algorithm>
#include <iostream>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
    ListTemperatureScheduler::ListTemperatureScheduler () :
        adapt_counter(0), iterations_count(1000), length(90), i(0), adapt_sum(0.f), initial_probability(-20.72f)
    {
    }

    ListTemperatureScheduler::ListTemperatureScheduler(size_t aLength, float aInitialProbability, size_t iterationsCount) :
        adapt_counter(0), iterations_count(iterationsCount), length(aLength), i(0), adapt_sum(0.f), initial_probability(aInitialProbability)
    {
    }

    void ListTemperatureScheduler::changeTemperature()
    {
        if (isFinish()) {
            return;
        }

        if (adapt_counter != 0) {
            values.pop();
            values.push(adapt_sum / adapt_counter);
        }
        adapt_counter = 0;
        adapt_sum = 0.f;
        ++i;
    }

    void ListTemperatureScheduler::initialize(Run* run, ScheduleCostFunction* cost_function)
    {
        std::priority_queue<float> new_values;
        const auto costs = create_initial_costs(run, cost_function, length);
        const auto best_cost_it = std::min_element(costs.begin(), costs.end());
        auto best_cost = *best_cost_it;
        for (const auto &cost : costs) {
            const float t = -fabs((cost - best_cost).getValue()) / this->initial_probability;
            new_values.push(t);
        }
        values = new_values;

        adapt_counter = 0;
        adapt_sum = 0.f;
        i = 0;
    }

    void ListTemperatureScheduler::adapt(Cost delta, float random)
    {
        const float value = -delta.getValue() / std::log(random);
        adapt_sum += value;
        ++adapt_counter;
    }

    float ListTemperatureScheduler::getTemperature() const
    {
        return values.top();
    }

    bool ListTemperatureScheduler::isFinish() const
    {
        return i >= iterations_count || getTemperature() == 0.f;
    }
}
