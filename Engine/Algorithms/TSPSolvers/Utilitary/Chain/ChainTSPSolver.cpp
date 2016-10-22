#include "ChainTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
    ChainTSPSolver::ChainTSPSolver()
	{
	}

    void ChainTSPSolver::optimize(Schedule& schedule) const
	{
        for (const TSPSolver& solver : tsp_solvers) {
            solver.optimize(schedule);
        }
	}

	void ChainTSPSolver::optimize(Run& run) const
	{
        for (const TSPSolver& solver : this->tsp_solvers) {
            solver.optimize(run);
        }
	}

    void ChainTSPSolver::addTSPSolver(const TSPSolver& aTSPSolver)
	{
        tsp_solvers.emplace_back(aTSPSolver);
	}
}
