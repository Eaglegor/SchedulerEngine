#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Schedule;

	class PerformerAvailabilityWindowsViolation : public ConstraintViolation
	{
	public:
		PerformerAvailabilityWindowsViolation(const Schedule& schedule)
		    : schedule(schedule)
		{
		}

		PerformerAvailabilityWindowsViolation(const PerformerAvailabilityWindowsViolation& rhs)
		    : schedule(rhs.schedule)
		{
		}

		const Schedule& getSchedule( ) const
		{
			return schedule;
		}

		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::PERFORMER_AVAILABILITY_WINDOWS_VIOLATION;
		}

	private:
		const Schedule& schedule;
	};
}