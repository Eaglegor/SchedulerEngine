#include "PerformerConstraints.h"

namespace Scheduler
{
	const PerformerConstraints::AvailabilityWindowsConstraint& PerformerConstraints::availabilityWindows( ) const
	{
		return availability_windows_constraint;
	}

	PerformerConstraints::AvailabilityWindowsConstraint& PerformerConstraints::availabilityWindows( )
	{
		return availability_windows_constraint;
	}

	const PerformerConstraints::DepotConstraint & PerformerConstraints::depot() const
	{
		return depot_constraint;
	}

	PerformerConstraints::DepotConstraint & PerformerConstraints::depot()
	{
		return depot_constraint;
	}

}
