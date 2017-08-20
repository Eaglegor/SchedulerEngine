#pragma once

#include <SceneManager_export.h>
#include "../ConstraintHolder.h"
#include <vector>
#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler
{
        class Depot;
    
	class SCENEMANAGER_EXPORT PerformerConstraints
	{
	public:
		using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;
                using DepotConstraint = ConstraintHolder<const Depot&>;

		const AvailabilityWindowsConstraint& availabilityWindows() const;
		AvailabilityWindowsConstraint& availabilityWindows();

                const DepotConstraint& depot() const;
                DepotConstraint& depot();
                
	private:
		AvailabilityWindowsConstraint availability_windows_constraint;
                DepotConstraint depot_constraint;
	};
}
