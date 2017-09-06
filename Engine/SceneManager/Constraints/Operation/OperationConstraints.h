#pragma once
#include <memory>
#include <vector>

#include "../ConstraintHolder.h"
#include <Engine/Concepts/Capacity.h>
#include <Engine/Utils/ReferenceWrapper.h>

#include <Engine/Concepts/TimeWindow.h>


namespace Scheduler
{
	class Attribute;

	class OperationConstraints
	{
	public:
		using TimeWindowsConstraint         = ConstraintHolder<std::vector<TimeWindow>>;
		using DemandConstraint              = ConstraintHolder<Capacity>;
		using PerformerSkillsRequirements   = ConstraintHolder<std::vector<ReferenceWrapper<const Attribute>>>;
		using VehicleAttributesRequirements = ConstraintHolder<std::vector<ReferenceWrapper<const Attribute>>>;

		const TimeWindowsConstraint& timeWindows( ) const;
		TimeWindowsConstraint& timeWindows( );

		const DemandConstraint& demand( ) const;
		DemandConstraint& demand( );

		const VehicleAttributesRequirements& vehicleAttributesRequirements( ) const;
		VehicleAttributesRequirements& vehicleAttributesRequirements( );

		const PerformerSkillsRequirements& performerSkillsRequirements( ) const;
		PerformerSkillsRequirements& performerSkillsRequirements( );

	private:
		TimeWindowsConstraint time_windows_constraint;
		DemandConstraint demand_constraint;
		VehicleAttributesRequirements vehicle_attributes_requirements_constraint;
		PerformerSkillsRequirements performer_skill_requirements_constraint;
	};
}
