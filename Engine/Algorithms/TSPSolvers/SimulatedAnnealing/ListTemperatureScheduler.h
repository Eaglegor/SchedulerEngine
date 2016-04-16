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
        ListTemperatureScheduler (size_t length, float initialProbability, size_t iterationsCount);

        virtual float getTemperature() const override;
        virtual bool isFinish() const override;

        virtual void adapt(float value) override;
        virtual void changeTemperature() override;
        virtual void initialize(Run* run, ScheduleCostFunction* cost_function, unsigned long long value) override;

        static constexpr const char* staticGetName() { return "Power"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }

    private:
        std::priority_queue<float> values;
        size_t adapt_counter;
        const size_t iterations_count;
        const size_t length;
        size_t i;
        float adapt_sum;
        const float initial_probability;
    };
}
