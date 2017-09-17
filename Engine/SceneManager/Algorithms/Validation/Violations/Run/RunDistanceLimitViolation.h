#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Distance.h>

namespace Scheduler
{
	class Run;

	/**
	 * @ingroup constraints
	 * 
	 * @brief Violation of the run distance limit constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class RunDistanceLimitViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run violating the constraint
		 * @param excess Amount of limit excess
		 */
		RunDistanceLimitViolation(const Run& run, const Distance excess)
		    : run(run),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		RunDistanceLimitViolation(const RunDistanceLimitViolation& rhs)
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
			return ConstraintViolationType::RUN_DISTANCE_LIMIT_VIOLATION;
		}

	private:
		const Run& run;
		Distance excess;
	};
}