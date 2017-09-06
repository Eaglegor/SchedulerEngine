#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>


namespace Scheduler
{
	class TransparentTSPSolver : public TSPSolver
	{
	public:
		virtual const char* getName( ) const;
		static const char* staticGetName( );

		virtual void optimize(Run& run) const;
		virtual void optimize(Schedule& schedule) const;
	};
}