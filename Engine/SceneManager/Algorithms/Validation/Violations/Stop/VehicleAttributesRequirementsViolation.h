#pragma once

#include "../ConstraintViolation.h"

namespace Scheduler
{
	class Attribute;
	class Operation;
	class Stop;
	
	class VehicleAttributesRequirementsViolation : public ConstraintViolation
	{
	public:
		VehicleAttributesRequirementsViolation(const Stop& stop, const Operation& operation, const Attribute& missing_attribute):
		stop(stop),
		operation(operation),
		missing_attribute(missing_attribute)
		{
		}
		
		VehicleAttributesRequirementsViolation(const VehicleAttributesRequirementsViolation &rhs):
		stop(rhs.stop),
		operation(rhs.operation),
		missing_attribute(rhs.missing_attribute)
		{
		}
		
		const Stop& getStop() const
		{
			return stop;
		}
		
		const Operation& getOperation() const
		{
			return operation;
		}
		
		const Attribute& getMissingAttribute() const
		{
			return missing_attribute;
		}
		
		virtual ConstraintViolationType getType() const override
		{
			return ConstraintViolationType::VEHICLE_ATTRIBUTES_REQUIREMENTS_VIOLATION;
		}
		
	private:
		const Stop& stop;
		const Operation& operation;
		const Attribute& missing_attribute;
	};
}