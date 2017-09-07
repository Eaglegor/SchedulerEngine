#pragma once

#include <Engine/AlgorithmsManager/VRPSolver.h>


namespace Scheduler
{
	class TransparentVRPSolver : public VRPSolver
	{
	public:
		virtual void optimize(Scene& scene) const override;

		static constexpr const char* staticGetName( )
		{
			return "Transparent";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};
	};
}