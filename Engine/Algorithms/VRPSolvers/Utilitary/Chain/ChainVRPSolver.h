#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <ChainVRPSolver_export.h>
#include <list>

namespace Scheduler
{
	class CHAINVRPSOLVER_EXPORT ChainVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene* scene) const override;

		void appendSolver(VRPSolver* solver);

	private:
		std::list<VRPSolver*> solvers_chain;
	};
}