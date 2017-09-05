#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/LoggingManager/LoggingManager.h>


namespace Scheduler
{
	class ScrambleTSPSolver : public TSPSolver
	{
	public:
		ScrambleTSPSolver( );

		virtual const char* getName( ) const;
		static const char* staticGetName( );

		virtual void optimize(Run& run) const;
		virtual void optimize(Schedule& schedule) const;

	private:
		Logger& logger;
	};
}