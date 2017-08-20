#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Schedule;

	class VehicleAvailabilityWindowsViolation : public ConstraintViolation
	{
	public:
		VehicleAvailabilityWindowsViolation(const Schedule& schedule)
		    : schedule(schedule)
		{
		}

		VehicleAvailabilityWindowsViolation(const VehicleAvailabilityWindowsViolation& rhs)
		    : schedule(rhs.schedule)
		{
		}

		const Schedule& getSchedule( ) const
		{
			return schedule;
		}

		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::VEHICLE_AVAILABILITY_WINDOWS_VIOLATION;
		}

	private:
		const Schedule& schedule;
	};
}