#pragma once

#include "../ConstraintHolder.h"


namespace Scheduler
{
	class Depot;

	class OrderConstraints
	{
	public:
		using DepotConstraint = ConstraintHolder<const Depot&>;

		const DepotConstraint& depot( ) const;
		DepotConstraint& depot( );

	private:
		DepotConstraint depot_constraint;
	};
}
