#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <Engine/LoggingManager/LoggingManager.h>

#include <cstdint>

namespace Scheduler
{
	class RotateTSPSolver : public TSPSolver
	{
	public:
		RotateTSPSolver( );

		virtual const char* getName( ) const;
		static const char* staticGetName( );

		virtual void optimize(Run& run) const;
		virtual void optimize(Schedule& schedule) const;

		void setRatio(float ratio);

	private:
		float ratio;
		Logger& logger;
	};
}