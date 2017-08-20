#include "TSPOnlyVRPSolver.h"
#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	TSPOnlyVRPSolver::TSPOnlyVRPSolver(const TSPSolver& tsp_solver)
	    : tsp_solver(tsp_solver)
	{
	}

	void TSPOnlyVRPSolver::optimize(Scene& scene) const
	{
		for(Schedule& schedule : scene.getSchedules( ))
		{
			tsp_solver.optimize(schedule);
		}
	}
}
