#include "SimpleTwoOptTSPSolver.h"

namespace Scheduler
{
	SimpleTwoOptTSPSolver::SimpleTwoOptTSPSolver():
		schedule_cost_function(nullptr)
	{
	}

	void SimpleTwoOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
	}

	void SimpleTwoOptTSPSolver::optimize(Schedule* schedule, size_t run_index) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
	}

	void SimpleTwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
