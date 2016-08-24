#include "TransparentTSPSolver.h"

namespace Scheduler
{
	const char* TransparentTSPSolver::getName() const
	{
		return staticGetName();
	}
	
	const char* TransparentTSPSolver::staticGetName()
	{
		return "TransparentTSPSolver";
	}

	void TransparentTSPSolver::optimize(Schedule* schedule) const
	{
		// Do nothing
	}

	void TransparentTSPSolver::optimize(Run* run) const
	{
		// Do nothing
	}
}