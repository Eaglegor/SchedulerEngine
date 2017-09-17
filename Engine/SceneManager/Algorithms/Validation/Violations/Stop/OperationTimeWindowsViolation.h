#pragma once

#include "../ConstraintViolation.h"
#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	class Stop;

	/**
	 * @brief Violation of the operation time windows constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class OperationTimeWindowsViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param stop Stop violating the constraint
		 * @param excess Amount of late (>0) or early arrival (<0)
		 */
		OperationTimeWindowsViolation(const Stop& stop, const Duration& excess)
		    : stop(stop),
		      excess(excess)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		OperationTimeWindowsViolation(const OperationTimeWindowsViolation& rhs)
		    : stop(rhs.stop),
		      excess(rhs.excess)
		{
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
		 * @brief Returns the stop violating the constraint
		 * 
		 * @return Stop violating the constraint
		 */
		const Stop& getStop( ) const
		{
			return stop;
		}

	private:
		const Stop& stop;
		Duration excess;
	};
}