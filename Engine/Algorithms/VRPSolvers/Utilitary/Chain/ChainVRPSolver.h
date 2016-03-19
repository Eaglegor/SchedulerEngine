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

		static constexpr const char* staticGetName() { return "Chain"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		std::list<VRPSolver*> solvers_chain;
	};
}