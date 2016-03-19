#include "ChainVRPSolver.h"

namespace Scheduler
{
	void ChainVRPSolver::optimize(Scene* scene) const
	{
		for(VRPSolver* solver : solvers_chain)
		{
			solver->optimize(scene);
		}
	}

	void ChainVRPSolver::appendSolver(VRPSolver* solver)
	{
		solvers_chain.push_back(solver);
	}
}
