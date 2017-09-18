#pragma once

#include "../ConstraintHolder.h"


namespace Scheduler
{
	class Depot;

	/**
	 * @brief Order constraints set
	 */
	class OrderConstraints
	{
	public:
		/**
		 * @brief Depot binding constraint
		 */
		using DepotConstraint = ConstraintHolder<const Depot&>;

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
		DepotConstraint depot_constraint;
	};
}
