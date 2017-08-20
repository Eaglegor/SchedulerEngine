#pragma once
#include <memory>
#include <vector>

#include <SceneManager_export.h>
#include <Engine/Concepts/TimeWindow.h>
#include "../ConstraintHolder.h"

namespace Scheduler
{
	class Capacity;
        class Depot;

	class SCENEMANAGER_EXPORT VehicleConstraints
	{
		public:
			using CapacityConstraint = ConstraintHolder<Capacity>;
			using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;
                        using DepotConstraint = ConstraintHolder<const Depot&>;

			const CapacityConstraint& capacity() const;
			CapacityConstraint& capacity();

			const AvailabilityWindowsConstraint& availabilityWindows() const;
			AvailabilityWindowsConstraint& availabilityWindows();
                        
                        const DepotConstraint& depot() const;
                        DepotConstraint& depot();

		private:
			CapacityConstraint capacity_constraint;
			AvailabilityWindowsConstraint availability_windows_constraint;
                        DepotConstraint depot_constraint;
	};
}
