#include "DurationLimitScheduleValidationAlgorithm.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>
#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>

namespace Scheduler
{
	void DurationLimitScheduleValidationAlgorithm::validate(const Schedule& schedule, ViolationsConsumer& violations_consumer) const
	{
		if(!violations_consumer.supportsViolationType(ConstraintViolationType::SCHEDULE_WORKING_TIME_LIMIT_VIOLATION)) return;
		
		if(schedule.getRuns().empty()) return;
		if(!schedule.constraints().scheduleWorkingTimeLimit().isSet()) return;
		
		Duration schedule_duration = std::prev(schedule.getStops().end())->getAllocationTime().getEndTime() - 
										schedule.getStops().begin()->getAllocationTime().getStartTime();
										
		const Duration& schedule_duration_limit = schedule.constraints().scheduleWorkingTimeLimit();
		
		if(schedule_duration > schedule_duration_limit)
		{
			violations_consumer.consumeViolation(ScheduleWorkingTimeLimitViolation(schedule, schedule_duration - schedule_duration_limit));
		}
		
	}
}

