#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Schedule;

	class ScheduleWorkingTimeLimitViolation : public ConstraintViolation
	{
	public:
		ScheduleWorkingTimeLimitViolation(const Schedule& schedule, const Duration excess)
		    : schedule(schedule),
		      excess(excess)
		{
		}

		ScheduleWorkingTimeLimitViolation(const ScheduleWorkingTimeLimitViolation& rhs)
		    : schedule(rhs.schedule),
		      excess(rhs.excess)
		{
		}

		const Schedule& getSchedule( ) const
		{
			return schedule;
		}

		const Duration& getExcess( ) const
		{
			return excess;
		}

		virtual ConstraintViolationType getType( ) const
		{
			return ConstraintViolationType::SCHEDULE_WORKING_TIME_LIMIT_VIOLATION;
		}

	private:
		const Schedule& schedule;
		Duration excess;
	};
}