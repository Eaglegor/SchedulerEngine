#include "DistanceLimitScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DistanceAccumulator.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>
#include <algorithm>

namespace Scheduler
{
	void DistanceLimitScheduleValidationAlgorithm::validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const
	{
		if(schedule.getRuns().empty()) return;
		if(!schedule.constraints().scheduleDistanceLimit().isSet()) return;

		Distance schedule_distance = std::accumulate(schedule.getStops().begin(), schedule.getStops().end(), Distance(0), DistanceAccumulator());
		const Distance& schedule_distance_limit = schedule.constraints().scheduleDistanceLimit();
		
		if(schedule_distance > schedule_distance_limit)
		{
			violations_consumer.consumeViolation(ScheduleDistanceLimitViolation(schedule, schedule_distance - schedule_distance_limit));
		}
	}
}
