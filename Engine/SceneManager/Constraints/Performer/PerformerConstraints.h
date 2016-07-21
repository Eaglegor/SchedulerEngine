#pragma once

#include <SceneManager_export.h>
#include "../ConstraintHolder.h"
#include <vector>
#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler
{
	class SCENEMANAGER_EXPORT PerformerConstraints
	{
	public:
		using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;

		const AvailabilityWindowsConstraint& availabilityWindows() const;
		AvailabilityWindowsConstraint& availabilityWindows();

	private:
		AvailabilityWindowsConstraint availability_windows_constraint;
	};
}
