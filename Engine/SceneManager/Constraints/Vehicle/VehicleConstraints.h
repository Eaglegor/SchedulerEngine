#pragma once
#include <memory>
#include <vector>

#include "../ConstraintHolder.h"
#include <Engine/Concepts/TimeWindow.h>


namespace Scheduler
{
	class Capacity;
	class Depot;

	/**
	 * @ingroup constraints
	 * 
	 * @brief Vehicle constraints set
	 */
	class VehicleConstraints
	{
	public:
		/**
		 * @brief Vehicle capacity constraint
		 */
		using CapacityConstraint            = ConstraintHolder<Capacity>;

		/**
		 * @brief Vehicle availability windows constraint
		 */
		using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;

		/**
		 * @brief Depot binding constraint
		 */
		using DepotConstraint               = ConstraintHolder<const Depot&>;

		/**
		 * @brief Returns vehicle capacity constraint
		 * 
		 * @return Vehicle capacity constraint
		 */
		const CapacityConstraint& capacity( ) const;

		/**
		 * @brief Returns vehicle capacity constraint
		 * 
		 * @return Vehicle capacity constraint
		 */
		CapacityConstraint& capacity( );

		/**
		 * @brief Returns vehicle availability windows constraint
		 * 
		 * @return Vehicle availability windows constraint
		 */
		const AvailabilityWindowsConstraint& availabilityWindows( ) const;

		/**
		 * @brief Returns vehicle availability windows constraint
		 * 
		 * @return Vehicle availability windows constraint
		 */
		AvailabilityWindowsConstraint& availabilityWindows( );

		/**
		 * @brief Returns depot binding constraint
		 * 
		 * @return Depot binding constraint
		 */
		const DepotConstraint& depot( ) const;

		/**
		 * @brief Returns depot binding constraint
		 * 
		 * @return Depot binding constraint
		 */
		DepotConstraint& depot( );

	private:
		CapacityConstraint capacity_constraint;
		AvailabilityWindowsConstraint availability_windows_constraint;
		DepotConstraint depot_constraint;
	};
}
