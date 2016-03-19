#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <cstddef>
#include <SimpleTwoOptTSPSolver_export.h>

namespace Scheduler
{
	class SIMPLETWOOPTTSPSOLVER_EXPORT SimpleTwoOptTSPSolver : public TSPSolver
	{
	public:
		SimpleTwoOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Schedule* schedule, size_t run_index) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);

		static constexpr const char* staticGetName() { return "SimpleTwoOpt"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
	};
}