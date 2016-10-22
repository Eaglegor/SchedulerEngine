#include "TotalDistanceScheduleCostFunction.h"

#include "TotalDistanceRunCostFunction.h"
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	Cost TotalDistanceScheduleCostFunction::calculateCost(const Schedule& schedule) const
	{
		Cost total_cost(0);
		for(const Run& run : schedule.getRuns())
		{
			total_cost += TotalDistanceRunCostFunction().calculateCost(run);
		}
		return total_cost;
	}
}
