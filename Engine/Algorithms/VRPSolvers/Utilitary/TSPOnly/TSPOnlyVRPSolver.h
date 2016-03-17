#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <TSPOnlyVRPSolver.h>

namespace Scheduler
{
	class TSPSolver;

	class TSPONLYVRPSOLVER_EXPORT TSPOnlyVRPSolver : public VRPSolver
	{
	public:
		TSPOnlyVRPSolver(TSPSolver* tsp_solver);
		virtual void optimize(Scene* scene) override;

	private:
		TSPSolver* tsp_solver;
	};
}