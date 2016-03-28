#include "ChainTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
    ChainTSPSolver::ChainTSPSolver()
	{
	}

    void ChainTSPSolver::optimize(Schedule* schedule) const
	{
        for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

	void ChainTSPSolver::optimize(Run* run) const
	{
        for (auto solver : this->tsp_solvers) {
            solver->optimize(run);
        }
	}

    void ChainTSPSolver::addTSPSolver(TSPSolver* aTSPSolver)
	{
        this->tsp_solvers.push_back(aTSPSolver);
	}
}
