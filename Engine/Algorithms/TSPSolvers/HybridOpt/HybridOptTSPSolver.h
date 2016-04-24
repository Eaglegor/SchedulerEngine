#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <HybridOptTSPSolver_export.h>

namespace Scheduler
{
	class HYBRIDOPTTSPSOLVER_EXPORT HybridOptTSPSolver : public TSPSolver
	{
	public:
		HybridOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);

		static constexpr const char* staticGetName() { return "SimpleTwoOpt"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
	};
}
