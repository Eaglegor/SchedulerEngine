#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Schedule;

	/**
	 * @brief Violation of the schedule working time limit constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class ScheduleWorkingTimeLimitViolation : public ConstraintViolation
	{
	public:
		ScheduleWorkingTimeLimitViolation(const Schedule& schedule, const Duration excess)
		    : schedule(schedule),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		ScheduleWorkingTimeLimitViolation(const ScheduleWorkingTimeLimitViolation& rhs)
		    : schedule(rhs.schedule),
		      excess(rhs.excess)
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
		 * @brief Returns the amount of limit excess
		 * 
		 * @return Amount of limit excess
		 */
		const Duration& getExcess( ) const
		{
			return excess;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const
		{
			return ConstraintViolationType::SCHEDULE_WORKING_TIME_LIMIT_VIOLATION;
		}

	private:
		const Schedule& schedule;
		Duration excess;
	};
}