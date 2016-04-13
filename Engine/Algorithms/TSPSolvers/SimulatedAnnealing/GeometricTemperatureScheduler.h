#pragma once

#include "TemperatureScheduler.h"
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>

namespace Scheduler
{
    class Cost;

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT GeometricTemperatureScheduler : public TemperatureScheduler
    {
    public:
        GeometricTemperatureScheduler ();
        GeometricTemperatureScheduler (float p0, float p1, size_t iterationsCount);

        virtual float getTemperature() const override;
        virtual bool isFinish() const override;

        virtual void changeTemperature() override;
        virtual void initialize(Run* schedule, ScheduleCostFunction* cost_function, unsigned long long value) override;
        virtual void adapt(float value) override;

        static constexpr const char* staticGetName() { return "Power"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }
    private:
        void initialize(float avgCost);

        const float p0;
        const float p1;
        const size_t iterations_count;

        float initial_value;
        float minumum_value;
        float change_speed;
        float current_value;
    };
}
