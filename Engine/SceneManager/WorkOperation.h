#pragma once

#include "Operation.h"



namespace Scheduler
{
	class WorkOperation : public Operation
	{
	public:
		WorkOperation(std::size_t id, const Location& location, const Order& order);

		const Location& getLocation( ) const;

	private:
		const Location& location;
	};
}
