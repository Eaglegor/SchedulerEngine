#include "TSPOnlyVRPSolver.h"
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/Strategies/TSPSolver.h>

namespace Scheduler
{
	TSPOnlyVRPSolver::TSPOnlyVRPSolver(TSPSolver* tsp_solver):
		tsp_solver(tsp_solver)
	{
		assert(tsp_solver);
	}

	void TSPOnlyVRPSolver::optimize(Scene* scene)
	{
		assert(tsp_solver);
		for(Schedule* schedule : scene->getSchedules())
		{
			tsp_solver->optimize(schedule);
		}
	}
}
