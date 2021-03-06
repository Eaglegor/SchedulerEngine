#include "DurationLimitRunValidationAlgorithm.h"

#include <Engine/SceneManager/Algorithms/Validation/ViolationsConsumer.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Utils/Aggregators/DurationAccumulator.h>

namespace Scheduler
{
	void DurationLimitRunValidationAlgorithm::validate(const Run& run, ViolationsConsumer& violations_consumer) const
	{
		if(!violations_consumer.supportsViolationType(ConstraintViolationType::RUN_WORKING_TIME_LIMIT_VIOLATION)) return;

		if(!run.getSchedule( ).constraints( ).runWorkingTimeLimit( ).isSet( )) return;

		Duration run_duration              = run.getEndStop( ).getAllocationTime( ).getEndTime( ) - run.getStartStop( ).getAllocationTime( ).getStartTime( );
		const Duration& run_duration_limit = run.getSchedule( ).constraints( ).runWorkingTimeLimit( );

		if(run_duration > run_duration_limit)
		{
			violations_consumer.consumeViolation(RunWorkingTimeLimitViolation(run, run_duration - run_duration_limit));
		}
	}
}
