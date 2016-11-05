#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Schedule;
	
	class ScheduleDrivingTimeLimitViolation : public ConstraintViolation
	{
	public:
		ScheduleDrivingTimeLimitViolation(const Schedule& schedule, const Duration excess):
		schedule(schedule),
		excess(excess)
		{
		}
		
		ScheduleDrivingTimeLimitViolation(const ScheduleDrivingTimeLimitViolation& rhs):
		schedule(rhs.schedule),
		excess(rhs.excess)
		{
		}
		
		const Schedule& getSchedule() const
		{
			return schedule;
		}
		
		const Duration& getExcess() const
		{
			return excess;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::SCHEDULE_DRIVING_TIME_LIMIT_VIOLATION;
		}
		
	private:
		const Schedule& schedule;
		Duration excess;
	};
}