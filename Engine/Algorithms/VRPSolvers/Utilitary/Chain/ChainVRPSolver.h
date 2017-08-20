#pragma once

#include <ChainVRPSolver_export.h>
#include <Engine/AlgorithmsManager/VRPSolver.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <list>

namespace Scheduler
{
	class CHAINVRPSOLVER_EXPORT ChainVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene& scene) const override;

		void appendSolver(const VRPSolver& solver);

		static constexpr const char* staticGetName( )
		{
			return "Chain";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

	private:
		std::list<ReferenceWrapper<const VRPSolver>> solvers_chain;
	};
}