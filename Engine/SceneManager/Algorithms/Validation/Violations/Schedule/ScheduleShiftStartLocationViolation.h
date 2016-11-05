#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Schedule;
	class Location;
	
	class ScheduleShiftStartLocationViolation : public ConstraintViolation
	{
	public:
		ScheduleShiftStartLocationViolation(const Schedule& schedule, const Location& expected_location, const Location& actual_location):
		schedule(schedule),
		expected_location(expected_location),
		actual_location(actual_location)
		{
		}
		
		ScheduleShiftStartLocationViolation(const ScheduleShiftStartLocationViolation& rhs):
		schedule(rhs.schedule),
		expected_location(rhs.expected_location),
		actual_location(rhs.actual_location)
		{
		}
		
		const Schedule& getSchedule() const
		{
			return schedule;
		}
		
		const Location& getExpectedLocation() const
		{
			return expected_location;
		}
		
		const Location& getActualLocation() const
		{
			return actual_location;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::SCHEDULE_SHIFT_START_LOCATION_VIOLATION;
		}
		
	private:
		const Schedule& schedule;
		const Location& expected_location;
		const Location& actual_location;
	};
}