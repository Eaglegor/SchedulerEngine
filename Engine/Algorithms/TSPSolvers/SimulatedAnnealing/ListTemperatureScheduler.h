#pragma once

#include "TemperatureScheduler.h"
#include <queue>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>

namespace Scheduler
{
    class Cost;

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT ListTemperatureScheduler : public TemperatureScheduler
    {
    public:
        ListTemperatureScheduler ();
        ListTemperatureScheduler (std::size_t length, float initialProbability, std::size_t iterationsCount);

        virtual float getTemperature() const override;
        virtual bool isFinish() const override;

        virtual void adapt(Cost delta, float random) override;
        virtual void changeTemperature() override;
        virtual void initialize(Run& run, const ScheduleCostFunction& cost_function) override;

        static constexpr const char* staticGetName() { return "List"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }

    private:
        std::priority_queue<float> values;
        std::size_t adapt_counter;
        const size_t iterations_count;
        const size_t length;
        std::size_t i;
        float adapt_sum;
        const float initial_probability;
    };
}
