#include "WorkOperation.h"

namespace Scheduler
{
	WorkOperation::WorkOperation(std::size_t id, const Location& location, const Order& order)
	    : Operation(id, order),
	      location(location)
	{
	}

	const Location& WorkOperation::getLocation( ) const
	{
		return location;
	}
}
