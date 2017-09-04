#pragma once


#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <vector>

namespace Scheduler
{
	class ChainTSPSolver : public TSPSolver
	{
	public:
		ChainTSPSolver( );

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& schedule) const override;

		void addTSPSolver(const TSPSolver& aTSPSolver);

		static constexpr const char* staticGetName( )
		{
			return "Chain";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		std::vector<ReferenceWrapper<const TSPSolver>> tsp_solvers;
	};
}
