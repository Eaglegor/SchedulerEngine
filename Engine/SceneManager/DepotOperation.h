#pragma once

#include "Operation.h"



namespace Scheduler
{
	class DepotOperation : public Operation
	{
	public:
		DepotOperation(std::size_t id, const Order& order);
	};
}
