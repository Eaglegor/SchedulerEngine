#pragma once

#include <Engine/StrategiesManager/VRPSolver.h>
#include <SweepVRPSolver.h>

namespace Scheduler
{
	class TSPSolver;

    class SWEEPVRPSOLVER_EXPORT SweepVRPSolver : public VRPSolver
	{
	public:
        SweepVRPSolver();
        ~SweepVRPSolver();
		virtual void optimize(Scene* scene) const override;

		static constexpr const char* staticGetName() { return "Sweep"; }
		virtual const char* getName() const override { return staticGetName(); };
	};
}