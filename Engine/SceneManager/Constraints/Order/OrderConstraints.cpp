#include "OrderConstraints.h"

namespace Scheduler
{
	const OrderConstraints::DepotConstraint& Scheduler::OrderConstraints::depot( ) const
	{
		return depot_constraint;
	}

	OrderConstraints::DepotConstraint& Scheduler::OrderConstraints::depot( )
	{
		return depot_constraint;
	}
}
