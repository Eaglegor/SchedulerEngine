#pragma once

#include "../ConstraintHolder.h"
#include <Engine/Concepts/TimeWindow.h>

#include <vector>

namespace Scheduler
{
	class Depot;

	class PerformerConstraints
	{
	public:
		using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;
		using DepotConstraint               = ConstraintHolder<const Depot&>;

		const AvailabilityWindowsConstraint& availabilityWindows( ) const;
		AvailabilityWindowsConstraint& availabilityWindows( );

		const DepotConstraint& depot( ) const;
		DepotConstraint& depot( );

	private:
		AvailabilityWindowsConstraint availability_windows_constraint;
		DepotConstraint depot_constraint;
	};
}
