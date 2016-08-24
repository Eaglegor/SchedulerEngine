#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <ReverseTSPSolver_export.h>

namespace Scheduler
{
	class REVERSETSPSOLVER_EXPORT ReverseTSPSolver : public TSPSolver
	{
	public:
		virtual const char* getName() const;
		static const char* staticGetName();
		
		virtual void optimize(Run* run) const;
		virtual void optimize(Schedule* schedule) const;
	};
}