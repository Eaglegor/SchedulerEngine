#include "ListTemperatureScheduler.h"
#include <numeric>
#include <Engine/Concepts/Cost.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
    ListTemperatureScheduler::ListTemperatureScheduler () :
        max_iterations_number(1000), values_length(90), iteration_counter(0), initial_probability(-20.72f)
    {
    }

    ListTemperatureScheduler::ListTemperatureScheduler(std::size_t aLength, float aInitialProbability, std::size_t iterationsCount) :
        max_iterations_number(iterationsCount), values_length(aLength), iteration_counter(0), initial_probability(aInitialProbability)
    {
    }

    void ListTemperatureScheduler::changeTemperature()
    {
        if (isFinish()) {
            return;
        }

        if (!adapt_values.empty()) {
            values.pop();
            values.push(new_value());
        }

        adapt_values.clear();
        ++iteration_counter;
    }

    void ListTemperatureScheduler::initialize(Run& run, const ScheduleCostFunction& cost_function)
    {
        std::priority_queue<float> new_values;
        const auto costs = create_initial_costs(run, cost_function, values_length);
        auto best_cost = *costs.begin();
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
        const bool finish = (iteration_counter >= max_iterations_number || getTemperature() <= 0.f);
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

    FastTemperatureScheduler::FastTemperatureScheduler() : ListTemperatureScheduler(), first_ratio(1.f)
    {}

    FastTemperatureScheduler::FastTemperatureScheduler(std::size_t aLength, float aInitialProbability, std::size_t iterationsCount, float quality) :
        ListTemperatureScheduler(aLength, aInitialProbability, iterationsCount), first_ratio(quality)
    {}

    float FastTemperatureScheduler::new_value()
    {
        if (adapt_values.empty()) {
            return 0.f;
        }

        std::size_t ofs = first_ratio * adapt_values.size();
        ofs = std::min(adapt_values.size(), ofs);
        ofs = std::max(std::size_t(1), ofs);
        std::partial_sort(adapt_values.begin(), std::next(adapt_values.begin(), ofs), adapt_values.end());
        const float new_value = std::accumulate(adapt_values.begin(), std::next(adapt_values.begin(), ofs), 0.f) / ofs;
        return new_value;
    }

    FastTemperatureScheduler* FastTemperatureScheduler::clone() const
    {
        return new FastTemperatureScheduler(*this);
    }

    SlowTemperatureScheduler::SlowTemperatureScheduler() : ListTemperatureScheduler(), first_ratio(0.f)
    {}

    SlowTemperatureScheduler::SlowTemperatureScheduler(std::size_t aLength, float aInitialProbability, std::size_t iterationsCount, float speed) :
        ListTemperatureScheduler(aLength, aInitialProbability, iterationsCount), first_ratio(speed)
    {}

    float SlowTemperatureScheduler::new_value()
    {
        if (adapt_values.empty()) {
            return 0.f;
        }

        std::size_t ofs = first_ratio * adapt_values.size();
        ofs = std::min(adapt_values.size(), ofs);
        ofs = std::max(std::size_t(1), ofs);

        std::partial_sort(adapt_values.begin(), std::next(adapt_values.begin(), ofs), adapt_values.end(), std::greater<float>());
        const float new_value = std::accumulate(adapt_values.begin(), std::next(adapt_values.begin(), ofs), 0.f) / ofs;
        return new_value;
    }

    SlowTemperatureScheduler* SlowTemperatureScheduler::clone() const
    {
        return new SlowTemperatureScheduler(*this);
    }
}
