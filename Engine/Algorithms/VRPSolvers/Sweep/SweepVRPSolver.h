#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <SweepVRPSolver.h>

namespace Scheduler
{
	class TSPSolver;

	class SweepVRPSolver : public VRPSolver
	{
	public:
		SweepVRPSolver( );
		~SweepVRPSolver( );
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName( )
		{
			return "Sweep";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};
	};
}
