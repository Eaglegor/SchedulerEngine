#pragma once
#include <vector>
#include <memory>

#include "../ConstraintHolder.h"
#include <Engine/Concepts/Capacity.h>

#include <SceneManager_export.h>
#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler
{
	class Attribute;

	class SCENEMANAGER_EXPORT OperationConstraints
	{
	public:
		using TimeWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;
		using DemandConstraint = ConstraintHolder<Capacity>;
		using PerformerSkillsRequirements = ConstraintHolder<std::vector<const Attribute*>>;
		using VehicleAttributesRequirements = ConstraintHolder<std::vector<const Attribute*>>;

		const TimeWindowsConstraint& timeWindows() const;
		TimeWindowsConstraint& timeWindows();

		const DemandConstraint& demand() const;
		DemandConstraint& demand();

		const VehicleAttributesRequirements& vehicleAttributesRequirements() const;
		VehicleAttributesRequirements& vehicleAttributesRequirements();

		const PerformerSkillsRequirements& performerSkillsRequirements() const;
		PerformerSkillsRequirements& performerSkillsRequirements();

	private:
		TimeWindowsConstraint time_windows_constraint;
		DemandConstraint demand_constraint;
		VehicleAttributesRequirements vehicle_attributes_requirements_constraint;
		PerformerSkillsRequirements performer_skill_requirements_constraint;
	};
}
