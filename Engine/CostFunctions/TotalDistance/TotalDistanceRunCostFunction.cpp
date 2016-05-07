#include "TotalDistanceRunCostFunction.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>

namespace Scheduler
{
	Cost TotalDistanceRunCostFunction::calculateCost(const Run* run)
	{
		return Cost(DistanceAccumulator::accumulateDistance(run->getStartStop(), run->getEndStop()).getValue());
	}
}
