#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>

namespace Scheduler
{
	class TSPSolver;

	class TSPOnlyVRPSolver : public VRPSolver
	{
	public:
		explicit TSPOnlyVRPSolver(const TSPSolver& tsp_solver);
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName( )
		{
			return "TSPOnly";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		const TSPSolver& tsp_solver;
	};
}