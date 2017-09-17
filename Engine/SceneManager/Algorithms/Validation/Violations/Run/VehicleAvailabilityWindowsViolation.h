#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Schedule;

	/**
	 * @brief Violation of the vehicle availability time windows constraint
	 */
	class VehicleAvailabilityWindowsViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @todo Add excess value
		 * 
		 * @param run Run violating the constraint
		 */
		VehicleAvailabilityWindowsViolation(const Schedule& schedule)
		    : schedule(schedule)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		VehicleAvailabilityWindowsViolation(const VehicleAvailabilityWindowsViolation& rhs)
		    : schedule(rhs.schedule)
		{
		}

		/**
		 * @brief Returns the run violating the constraint
		 * 
		 * @todo Should return run
		 * 
		 * @return Run violating the constraint
		 */
		const Schedule& getSchedule( ) const
		{
			return schedule;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::VEHICLE_AVAILABILITY_WINDOWS_VIOLATION;
		}

	private:
		const Schedule& schedule;
	};
}