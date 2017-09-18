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

	/**
	 * @brief Operation constraints set
	 */
	class OperationConstraints
	{
	public:
		/**
		 * @brief Time windows constraint
		 */
		using TimeWindowsConstraint         = ConstraintHolder<std::vector<TimeWindow>>;

		/**
		 * @brief Capacity demand constraint
		 */
		using DemandConstraint              = ConstraintHolder<Capacity>;

		/**
		 * @brief Performer skills requirements constraint
		 */
		using PerformerSkillsRequirements   = ConstraintHolder<std::vector<ReferenceWrapper<const Attribute>>>;

		/**
		 * @brief Vehicle attributes requirements constraint
		 */
		using VehicleAttributesRequirements = ConstraintHolder<std::vector<ReferenceWrapper<const Attribute>>>;

		/**
		 * @brief Returns time windows constraint
		 * 
		 * @return Time windows constraint
		 */
		const TimeWindowsConstraint& timeWindows( ) const;
		
		/**
		 * @brief Returns time windows constraint
		 * 
		 * @return Time windows constraint
		 */
		TimeWindowsConstraint& timeWindows( );

		/**
		 * @brief Returns demand constraint
		 * 
		 * @return Demand constraint
		 */
		const DemandConstraint& demand( ) const;

		/**
		 * @brief Returns demand constraint
		 * 
		 * @return Demand constraint
		 */
		DemandConstraint& demand( );

		/**
		 * @brief Returns vehicle attributes requiments constraint
		 * 
		 * @return Vehicle attributes requirements constraint
		 */
		const VehicleAttributesRequirements& vehicleAttributesRequirements( ) const;

		/**
		 * @brief Returns vehicle attributes requiments constraint
		 * 
		 * @return Vehicle attributes requirements constraint
		 */
		VehicleAttributesRequirements& vehicleAttributesRequirements( );

		/**
		 * @brief Returns performer skills requiments constraint
		 * 
		 * @return Performer skills requirements constraint
		 */
		const PerformerSkillsRequirements& performerSkillsRequirements( ) const;

		/**
		 * @brief Returns performer skills requiments constraint
		 * 
		 * @return Performer skills requirements constraint
		 */
		PerformerSkillsRequirements& performerSkillsRequirements( );

	private:
		TimeWindowsConstraint time_windows_constraint;
		DemandConstraint demand_constraint;
		VehicleAttributesRequirements vehicle_attributes_requirements_constraint;
		PerformerSkillsRequirements performer_skill_requirements_constraint;
	};
}
