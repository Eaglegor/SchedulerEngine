#pragma once

#include <vector>
#include <cstddef>

namespace Scheduler
{
    class Run;
    class ScheduleCostFunction;
    class Cost;

    class TemperatureScheduler
    {
    public:
        virtual ~TemperatureScheduler()
        {
        }

        virtual float getTemperature() const = 0;
        virtual bool isFinish() const = 0;
        virtual const char* getName() const = 0;

        virtual void adapt (Cost delta, float random) = 0;
        virtual void changeTemperature() = 0;
        virtual void initialize(Run* run, ScheduleCostFunction* cost_function) = 0;

    protected:
        std::vector<Cost> create_initial_costs(Run* run, ScheduleCostFunction* cost_function, size_t length);
    };
}
