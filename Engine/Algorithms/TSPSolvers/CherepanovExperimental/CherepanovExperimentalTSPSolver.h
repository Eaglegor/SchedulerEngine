#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <CherepanovExperimentalTSPSolver_export.h>

namespace Scheduler
{
    class CHEREPANOVEXPERIMENTALTSPSOLVER_EXPORT CherepanovExperimentalTSPSolver : public TSPSolver
	{
	public:
		CherepanovExperimentalTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);

        static constexpr const char* staticGetName() { return "CherepanovExperimental"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
	};
}
