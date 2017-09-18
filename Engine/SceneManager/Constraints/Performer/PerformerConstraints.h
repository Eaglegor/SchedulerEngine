#pragma once

#include "../ConstraintHolder.h"
#include <Engine/Concepts/TimeWindow.h>

#include <vector>

namespace Scheduler
{
	class Depot;

	/**
	 * @brief Performer constraints set
	 */
	class PerformerConstraints
	{
	public:
		/**
		 * @brief Performer availability windows constraint
		 */
		using AvailabilityWindowsConstraint = ConstraintHolder<std::vector<TimeWindow>>;

		/**
		 * @brief Depot binding constraint
		 */
		using DepotConstraint               = ConstraintHolder<const Depot&>;

		/**
		 * @brief Returns performer availability windows constraint
		 * 
		 * @return Performer availability windows constraint
		 */
		const AvailabilityWindowsConstraint& availabilityWindows( ) const;

		/**
		 * @brief Returns performer availability windows constraint
		 * 
		 * @return Performer availability windows constraint
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
		AvailabilityWindowsConstraint availability_windows_constraint;
		DepotConstraint depot_constraint;
	};
}
