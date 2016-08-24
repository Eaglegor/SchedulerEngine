#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <TransparentTSPSolver_export.h>

namespace Scheduler
{
	class TRANSPARENTTSPSOLVER_EXPORT TransparentTSPSolver : public TSPSolver
	{
	public:
		virtual const char* getName() const;
		static const char* staticGetName();
		
		virtual void optimize(Run* run) const;
		virtual void optimize(Schedule* schedule) const;
	};
}