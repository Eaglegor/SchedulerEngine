#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Schedule;

	/**
	 * @brief Violation of the performer availability time windows constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class PerformerAvailabilityWindowsViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param schedule Schedule violating the constraint
		 */
		PerformerAvailabilityWindowsViolation(const Schedule& schedule)
		    : schedule(schedule)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		PerformerAvailabilityWindowsViolation(const PerformerAvailabilityWindowsViolation& rhs)
		    : schedule(rhs.schedule)
		{
		}

		/**
		 * @brief Returns the schedule violating the constraint
		 * 
		 * @return Schedule violating the constraint
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
			return ConstraintViolationType::PERFORMER_AVAILABILITY_WINDOWS_VIOLATION;
		}

	private:
		const Schedule& schedule;
	};
}