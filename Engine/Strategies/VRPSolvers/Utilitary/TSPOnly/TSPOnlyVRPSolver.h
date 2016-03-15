#pragma once

#include <Engine/Strategies/VRPSolver.h>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class TSPSolver;

	class SCHEDULERENGINE_EXPORT TSPOnlyVRPSolver : public VRPSolver
	{
	public:
		TSPOnlyVRPSolver(TSPSolver* tsp_solver);
		virtual void optimize(Scene* scene) override;

	private:
		TSPSolver* tsp_solver;
	};
}