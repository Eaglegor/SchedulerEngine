#include "VehicleConstraints.h"
#include <Engine/Concepts/Capacity.h>

namespace Scheduler
{
	const Capacity * VehicleConstraints::capacity() const
	{
		return capacity_constraint.get();
	}

	void VehicleConstraints::addConstraint(const Capacity& capacity)
	{
		capacity_constraint = std::make_unique<Capacity>(capacity);
	}

	void VehicleConstraints::removeCapacityConstraint()
	{
		capacity_constraint.reset();
	}
}