#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Run;

	/**
	 * @brief Violation of the run working time limit constraint
	 */
	class RunWorkingTimeLimitViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run violating the constraint
		 * @param excess Amount of limit excess
		 */
		RunWorkingTimeLimitViolation(const Run& run, const Duration excess)
		    : run(run),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		RunWorkingTimeLimitViolation(const RunWorkingTimeLimitViolation& rhs)
		    : run(rhs.run),
		      excess(rhs.excess)
		{
		}

		/**
		 * @brief Returns the run violating the constraint
		 * 
		 * @todo rename into getRun
		 * 
		 * @return Run violating the constraint
		 */
		const Run& getSchedule( ) const
		{
			return run;
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
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::RUN_WORKING_TIME_LIMIT_VIOLATION;
		}

	private:
		const Run& run;
		Duration excess;
	};
}