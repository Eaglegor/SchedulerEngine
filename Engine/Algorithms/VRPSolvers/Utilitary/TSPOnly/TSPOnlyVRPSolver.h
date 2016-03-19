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
		virtual void optimize(Scene* scene) const override;

		static constexpr const char* staticGetName() { return "TSPOnly"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		TSPSolver* tsp_solver;
	};
}