#include "ChainVRPSolver.h"

namespace Scheduler
{
	void ChainVRPSolver::optimize(Scene& scene) const
	{
		for(const VRPSolver& solver : solvers_chain)
		{
			solver.optimize(scene);
		}
	}

	void ChainVRPSolver::appendSolver(const VRPSolver& solver)
	{
		solvers_chain.emplace_back(solver);
	}
}
