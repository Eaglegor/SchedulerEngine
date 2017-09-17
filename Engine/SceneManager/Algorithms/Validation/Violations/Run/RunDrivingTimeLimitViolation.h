#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Run;

	/**
	 * @brief Violation of the run driving time limit constraint
	 */
	class RunDrivingTimeLimitViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run violating the constraint
		 * @param excess Amount of limit excess
		 */
		RunDrivingTimeLimitViolation(const Run& run, const Duration excess)
		    : run(run),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		RunDrivingTimeLimitViolation(const RunDrivingTimeLimitViolation& rhs)
		    : run(rhs.run),
		      excess(rhs.excess)
		{
		}

		/**
		 * @brief Returns the run violating the constraint
		 * 
		 * @return Run violating the constraint
		 */
		const Run& getRun( ) const
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
			return ConstraintViolationType::RUN_DRIVING_TIME_LIMIT_VIOLATION;
		}

	private:
		const Run& run;
		Duration excess;
	};
}