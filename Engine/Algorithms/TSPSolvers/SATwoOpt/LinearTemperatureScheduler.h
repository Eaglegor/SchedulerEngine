#pragma once

#include "TemperatureScheduler.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT LinearTemperatureScheduler : public TemperatureScheduler
    {
    public:
        LinearTemperatureScheduler ();
        LinearTemperatureScheduler (float initial_value, float minumum_value, float change_speed);

        virtual float getTemperature() const override;
        virtual float getInitialTemperature() const override;
        virtual float getMinimalTemperature() const override;
        virtual bool isFinish() const override;

        virtual void changeTemperature() override;
        virtual void reset() override;

        static constexpr const char* staticGetName() { return "Linear"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }
    private:
        float initial_value;
        float minumum_value;
        float change_speed;
        float current_value;
    };
}
