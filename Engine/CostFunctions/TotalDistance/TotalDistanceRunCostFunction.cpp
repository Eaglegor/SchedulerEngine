#include "TotalDistanceRunCostFunction.h"

#include <algorithm>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>

namespace Scheduler
{
	Cost TotalDistanceRunCostFunction::calculateCost(const Run& run) const
	{
		return Cost(DistanceAccumulator::accumulateDistance(run.getStops().begin(), run.getStops().end()).getValue());
	}
}
