#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <SimpleTwoOptTSPSolver_export.h>

namespace Scheduler
{
	class SIMPLETWOOPTTSPSOLVER_EXPORT SimpleTwoOptTSPSolver : public TSPSolver
	{
	public:
		virtual void optimize(Schedule* schedule) override;
		virtual void optimize(Schedule* schedule, size_t index) override;
	};
}