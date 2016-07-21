#pragma once
#include <memory>
#include <vector>

#include <SceneManager_export.h>
#include <Engine/Concepts/TimeWindow.h>
#include "../ConstraintHolder.h"

namespace Scheduler
{
	class Capacity;

	class SCENEMANAGER_EXPORT VehicleConstraints
	{
		public:
			using CapacityConstraint = ConstraintHolder<Capacity>;
			using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;

			const CapacityConstraint& capacity() const;
			CapacityConstraint& capacity();

			const AvailabilityWindowsConstraint& availabilityWindows() const;
			AvailabilityWindowsConstraint& availabilityWindows();

		private:
			CapacityConstraint capacity_constraint;
			AvailabilityWindowsConstraint availability_windows_constraint;
	};
}
