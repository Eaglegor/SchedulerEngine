#include "PerformerConstraints.h"

namespace Scheduler
{
	const PerformerConstraints::AvailabilityWindowsConstraint& PerformerConstraints::availabilityWindows() const
	{
		return availability_windows_constraint;
	}

	PerformerConstraints::AvailabilityWindowsConstraint& PerformerConstraints::availabilityWindows()
	{
		return availability_windows_constraint;
	}
}
