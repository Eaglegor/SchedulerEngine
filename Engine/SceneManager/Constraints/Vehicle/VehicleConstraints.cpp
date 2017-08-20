#include "VehicleConstraints.h"
#include <Engine/Concepts/Capacity.h>
#include <assert.h>

namespace Scheduler
{
	const VehicleConstraints::CapacityConstraint& VehicleConstraints::capacity() const
	{
		return capacity_constraint;
	}

	VehicleConstraints::CapacityConstraint& VehicleConstraints::capacity()
	{
		return capacity_constraint;
	}

	const VehicleConstraints::AvailabilityWindowsConstraint& VehicleConstraints::availabilityWindows() const
	{
		return availability_windows_constraint;
	}

	VehicleConstraints::AvailabilityWindowsConstraint& VehicleConstraints::availabilityWindows()
	{
		return availability_windows_constraint;
	}
	
	const VehicleConstraints::DepotConstraint& VehicleConstraints::depot() const
	{
		return depot_constraint;
	}

	VehicleConstraints::DepotConstraint& VehicleConstraints::depot()
	{
		return depot_constraint;
	}
}
