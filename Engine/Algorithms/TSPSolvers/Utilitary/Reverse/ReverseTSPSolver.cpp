#include "ReverseTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	const char* ReverseTSPSolver::getName() const
	{
		return staticGetName();
	}
	
	const char* ReverseTSPSolver::staticGetName()
	{
		return "Reverse";
	}

	void ReverseTSPSolver::optimize(Schedule& schedule) const
	{
		for(Run& r : schedule.getRuns())
		{
			optimize(r);
		}
	}

	void ReverseTSPSolver::optimize(Run& run) const
	{
		run.reverseWorkStops();
	}
}