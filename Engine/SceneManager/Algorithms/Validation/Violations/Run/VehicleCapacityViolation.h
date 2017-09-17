#pragma once

#include "../ConstraintViolation.h"
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	/**
	 * @ingroup constraints
	 * 
	 * @brief Violation of the vehicle capacity constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class VehicleCapacityViolation : public ConstraintViolation
	{
	public:
		/**
		 * @brief Constructor
		 * 
		 * @param run Run violating the constraint
		 * @param excess Amount of limit excess
		 * @param pos Iterator to the stop that caused vehicle overload
		 */
		VehicleCapacityViolation(const Run& run, const Capacity& excess, Run::ConstStopIterator pos)
		    : run(run),
		      excess(excess),
		      pos(pos)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		VehicleCapacityViolation(const VehicleCapacityViolation& rhs)
		    : run(rhs.run),
		      excess(rhs.excess),
		      pos(rhs.pos)
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
		const Capacity& getExcess( ) const
		{
			return excess;
		}

		/**
		 * @brief Returns the iterator to the stop caused vehicle overload
		 * 
		 * @return Iterator to the stop caused vehicle overload
		 */
		Run::ConstStopIterator getExcessPosition( ) const
		{
			return pos;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::VEHICLE_CAPACITY_VIOLATION;
		}

	private:
		const Run& run;
		Capacity excess;
		Run::ConstStopIterator pos;
	};
}