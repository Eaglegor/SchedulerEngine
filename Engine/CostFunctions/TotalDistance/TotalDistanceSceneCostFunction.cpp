#include "TotalDistanceSceneCostFunction.h"

#include "TotalDistanceScheduleCostFunction.h"
#include <Engine/SceneManager/Scene.h>

namespace Scheduler
{
	Cost TotalDistanceSceneCostFunction::calculateCost(const Scene* scene)
	{
		Cost total_cost(0);
		for (const Schedule* schedule: scene->getSchedules())
		{
			total_cost += TotalDistanceScheduleCostFunction().calculateCost(schedule);
		}
		return total_cost;
	}
}
