#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Distance.h>

namespace Scheduler
{
	class Schedule;

	/**
	 * @ingroup constraints
	 * 
	 * @brief Violation of the schedule distance limit constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class ScheduleDistanceLimitViolation : public ConstraintViolation
	{
	public:
		ScheduleDistanceLimitViolation(const Schedule& schedule, const Distance& excess)
		    : schedule(schedule),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		ScheduleDistanceLimitViolation(const ScheduleDistanceLimitViolation& rhs)
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
		const Distance& getExcess( ) const
		{
			return excess;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::SCHEDULE_DISTANCE_LIMIT_VIOLATION;
		}

	private:
		const Schedule& schedule;
		Distance excess;
	};
}