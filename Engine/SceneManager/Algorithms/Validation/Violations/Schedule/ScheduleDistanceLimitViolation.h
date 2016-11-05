#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Distance.h>

namespace Scheduler
{
	class Schedule;
	
	class ScheduleDistanceLimitViolation : public ConstraintViolation
	{
	public:
		ScheduleDistanceLimitViolation(const Schedule& schedule, const Distance& excess):
		schedule(schedule),
		excess(excess)
		{
		}
		
		ScheduleDistanceLimitViolation(const ScheduleDistanceLimitViolation& rhs):
		schedule(rhs.schedule),
		excess(rhs.excess)
		{
		}
		
		const Schedule& getSchedule() const
		{
			return schedule;
		}
		
		const Distance& getExcess() const
		{
			return excess;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::SCHEDULE_DISTANCE_LIMIT_VIOLATION;
		}
		
	private:
		const Schedule& schedule;
		Distance excess;
	};
}