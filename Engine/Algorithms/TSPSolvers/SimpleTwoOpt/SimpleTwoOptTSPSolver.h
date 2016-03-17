#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <cstddef>
#include <SimpleTwoOptTSPSolver_export.h>

namespace Scheduler
{
	class SIMPLETWOOPTTSPSOLVER_EXPORT SimpleTwoOptTSPSolver : public TSPSolver
	{
	public:
		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Schedule* schedule, size_t index) const override;
	};
}