#include "DepotOperation.h"

namespace Scheduler
{
	DepotOperation::DepotOperation(std::size_t id, const Order& order)
	    : Operation(id, order)
	{
	}
}
