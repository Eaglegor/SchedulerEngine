#pragma once

#include <Engine/Strategies/VRPSolver.h>
#include <SchedulerEngine_export.h>
#include <list>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT ChainVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene* scene) override;

		void appendSolver(VRPSolver* solver);

	private:
		std::list<VRPSolver*> solvers_chain;
	};
}