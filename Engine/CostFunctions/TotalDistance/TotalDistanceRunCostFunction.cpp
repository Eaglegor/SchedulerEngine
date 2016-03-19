#include "TotalDistanceRunCostFunction.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Views/RunStopsView.h>

namespace Scheduler
{
	Cost TotalDistanceRunCostFunction::calculateCost(const Run* run)
	{
		Distance total_distance(0);

		for(const Stop* stop : ConstRunStopsView(run))
		{
			total_distance += stop->getNextRoute().getDistance();
		}

		return Cost(total_distance.getValue());
	}
}
