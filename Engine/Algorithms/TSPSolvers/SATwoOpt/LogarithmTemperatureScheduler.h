#pragma once

#include "TemperatureScheduler.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT LogarithmTemperatureScheduler : public TemperatureScheduler
    {
    public:
        LogarithmTemperatureScheduler ();
        LogarithmTemperatureScheduler (float initial_value, float minumum_value);

        virtual float getTemperature() const override;
        virtual float getInitialTemperature() const override;
        virtual float getMinimalTemperature() const override;
        virtual bool isFinish() const override;

        virtual void changeTemperature() override;
        virtual void reset() override;

        static constexpr const char* staticGetName() { return "Logarithm"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }
    private:
        float initial_value;
        float minumum_value;
        float current_value;
        unsigned long schedule_time;
    };
}
