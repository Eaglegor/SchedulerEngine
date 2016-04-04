#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <OneRelocateTSPSolver_export.h>

namespace Scheduler
{
    class ONERELOCATETSPSOLVER_EXPORT OneRelocateTSPSolver : public TSPSolver
	{
	public:
		OneRelocateTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);

        static constexpr const char* staticGetName() { return "OneRelocate"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
	};
}
