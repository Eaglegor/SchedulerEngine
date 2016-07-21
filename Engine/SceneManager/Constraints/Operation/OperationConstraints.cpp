#include "OperationConstraints.h"
#include <assert.h>

namespace Scheduler
{
	const OperationConstraints::TimeWindowsConstraint& OperationConstraints::timeWindows() const
	{
		return time_windows_constraint;
	}

	OperationConstraints::TimeWindowsConstraint& OperationConstraints::timeWindows()
	{
		return time_windows_constraint;
	}

	const OperationConstraints::DemandConstraint& OperationConstraints::demand() const
	{
		return demand_constraint;
	}

	OperationConstraints::DemandConstraint& OperationConstraints::demand()
	{
		return demand_constraint;
	}

	const OperationConstraints::VehicleAttributesRequirements& OperationConstraints::vehicleAttributesRequirements() const
	{
		return vehicle_attributes_requirements_constraint;
	}

	OperationConstraints::VehicleAttributesRequirements& OperationConstraints::vehicleAttributesRequirements()
	{
		return vehicle_attributes_requirements_constraint;
	}

	const OperationConstraints::PerformerSkillsRequirements& OperationConstraints::performerSkillsRequirements() const
	{
		return performer_skill_requirements_constraint;
	}

	OperationConstraints::PerformerSkillsRequirements& OperationConstraints::performerSkillsRequirements()
	{
		return performer_skill_requirements_constraint;
	}
}
