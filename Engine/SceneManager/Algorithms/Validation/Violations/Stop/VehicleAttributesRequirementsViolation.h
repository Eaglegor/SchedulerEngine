#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Attribute;
	class Operation;
	class Stop;

	/**
	 * @brief Violation of the required vehicle attribute order constraint
	 * 
	 * @sa @ref supported_constraints
	 */
	class VehicleAttributesRequirementsViolation : public ConstraintViolation
	{
	public:
		VehicleAttributesRequirementsViolation(const Stop& stop, const Operation& operation, const Attribute& missing_attribute)
		    : stop(stop),
		      operation(operation),
		      missing_attribute(missing_attribute)
		{
		}

		/**
		 * @brief Copy constructor
		 * 
		 * @param rhs Violation to copy the state from
		 */
		VehicleAttributesRequirementsViolation(const VehicleAttributesRequirementsViolation& rhs)
		    : stop(rhs.stop),
		      operation(rhs.operation),
		      missing_attribute(rhs.missing_attribute)
		{
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

		/**
		 * @brief Returns the operation constraint of which was violated
		 * 
		 * @return The operation constraint of which was violated
		 */
		const Operation& getOperation( ) const
		{
			return operation;
		}

		/**
		 * @brief Returns the missing required attribute
		 * 
		 * @return The missing required attribute
		 */
		const Attribute& getMissingAttribute( ) const
		{
			return missing_attribute;
		}

		/**
		 * @brief Returns the violation type
		 * 
		 * @return Violation type
		 */
		virtual ConstraintViolationType getType( ) const override
		{
			return ConstraintViolationType::VEHICLE_ATTRIBUTES_REQUIREMENTS_VIOLATION;
		}

	private:
		const Stop& stop;
		const Operation& operation;
		const Attribute& missing_attribute;
	};
}