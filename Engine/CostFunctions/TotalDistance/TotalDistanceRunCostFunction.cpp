#include "TotalDistanceRunCostFunction.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>
#include <numeric>

namespace Scheduler
{
	Cost TotalDistanceRunCostFunction::calculateCost(const Run& run) const
	{
		return Cost(std::accumulate(run.getStops( ).begin( ), run.getStops( ).end( ), Distance(0), DistanceAccumulator( )).getValue( ));
	}
}
