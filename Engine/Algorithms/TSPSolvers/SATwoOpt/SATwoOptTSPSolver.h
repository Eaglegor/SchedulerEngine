#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <SimpleTwoOptTSPSolver_export.h>
#include "AcceptanceFunction.h"

namespace Scheduler
{
    class SIMPLETWOOPTTSPSOLVER_EXPORT SATwoOptTSPSolver : public TSPSolver
	{
	public:
        SATwoOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setAcceptanceFunction(AcceptanceFunction* acceptance_function);

        static constexpr const char* staticGetName() { return "SATwoOpt"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
        AcceptanceFunction* acceptance_function;
	};
}
