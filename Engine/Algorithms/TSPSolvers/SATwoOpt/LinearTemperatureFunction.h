#pragma once

#include "TemperatureFunction.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT LinearTemperatureFunction : public TemperatureFunction
    {
    public:
        LinearTemperatureFunction ();
        LinearTemperatureFunction (float initial_value, float minumum_value, float change_speed);

        virtual float getTemperature() const override;
        virtual bool isFinish() const override;

        virtual void changeTemperature() override;
        virtual void reset() override;

        virtual const char* getName() const override
        {
            return "Linear";
        }
    private:
        float initial_value;
        float minumum_value;
        float change_speed;
        float current_value;
    };
}
