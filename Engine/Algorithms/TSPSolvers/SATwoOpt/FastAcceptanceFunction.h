#pragma once
#include "AcceptanceFunction.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT FastAcceptanceFunction : public AcceptanceFunction
    {
    public:
        virtual bool isAccepting(const Cost& delta, float temperature) const override;
        static constexpr const char* staticGetName() { return "VeryFast"; }
        virtual const char* getName() const override
        {
            return staticGetName();
        }
    };
}
