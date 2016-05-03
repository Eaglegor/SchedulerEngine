#include "TotalDistanceRunCostFunction.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>

namespace Scheduler
{
	Cost TotalDistanceRunCostFunction::calculateCost(const Run* run)
	{
		Distance total_distance(0);
		
		for (const Stop* stop = run->getStartStop(); stop != run->getEndStop(); stop = stop->getNextStop())
		{
			total_distance += stop->getNextRoute().getDistance();
		}

		return Cost(total_distance.getValue());
	}
}
