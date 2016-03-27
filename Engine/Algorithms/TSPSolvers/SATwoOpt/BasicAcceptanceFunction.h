#pragma once
#include "AcceptanceFunction.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT BasicAcceptanceFunction : public AcceptanceFunction
    {
    public:
        BasicAcceptanceFunction();
        BasicAcceptanceFunction(float initial_temperature, float minimum_temperature, float linear_temperature_change);
        virtual bool isAccepting(const Cost& delta, unsigned long long iterationNumber) const override;
        static constexpr const char* staticGetName() { return "Basic"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }
    private:
        float calculateTemperature (unsigned long long iterationNumber) const;
        float minimum_temperature;
        float initial_temperature;
        float linear_temperature_change;
    };
}
