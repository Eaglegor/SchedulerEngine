#include "ListTemperatureScheduler.h"
#include <numeric>
#include <Engine/Concepts/Cost.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
    ListTemperatureScheduler::ListTemperatureScheduler () :
        list_size(90), iterations_limit(1000), initial_probability(-20.72f), iteration_counter(0)
    {
    }

    ListTemperatureScheduler::ListTemperatureScheduler(std::size_t alength, std::size_t iterations_count, float ainitial_probability) :
        list_size(alength), iterations_limit(iterations_count), initial_probability(ainitial_probability), iteration_counter(0)
    {
    }

    void ListTemperatureScheduler::changeTemperature()
    {
        if (isFinish()) {
            return;
        }

        values.pop();
        values.push(new_value());
        adapt_values.clear();
        ++iteration_counter;
    }

    void ListTemperatureScheduler::initialize(Run& run, const ScheduleCostFunction& cost_function)
    {
        const auto costs = create_initial_costs(run, cost_function, list_size);
        auto best_cost = *costs.begin();
        std::priority_queue<float> new_values;
        for (const auto &cost : costs) {
            const float t = -fabs((cost - best_cost).getValue()) / this->initial_probability;
            new_values.push(t);
            if (cost < best_cost) {
                best_cost = cost;
            }
        }
        values = new_values;
        adapt_values.clear();
        iteration_counter = 0;
    }

    void ListTemperatureScheduler::adapt(Cost delta, float random)
    {
        const float value = -delta.getValue() / std::log(random);
        adapt_values.push_back(value);
    }


    float ListTemperatureScheduler::getTemperature() const
    {
        return values.top();
    }

    bool ListTemperatureScheduler::isFinish() const
    {
        const bool finish = (iteration_counter >= iterations_limit || getTemperature() <= 0.f);
        return finish;
    }

    float ListTemperatureScheduler::new_value()
    {
        if (adapt_values.empty()) {
            return 0.f;
        }

        const float new_value = std::accumulate(adapt_values.begin(), adapt_values.end(), 0.f) / adapt_values.size();
        return new_value;
    }

    ListTemperatureScheduler* ListTemperatureScheduler::clone() const
    {
        return new ListTemperatureScheduler(*this);
    }
}
