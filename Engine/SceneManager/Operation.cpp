#include "Operation.h"

namespace Scheduler
{
	Operation::Operation(std::size_t id, const Order& order)
	    : id(id),
	      order(order)
	{
	}

	const String& Operation::getName( ) const
	{
		return name;
	}

	std::size_t Operation::getId( ) const
	{
		return id;
	}

	const Duration& Operation::getDuration( ) const
	{
		return duration;
	}

	const Order& Operation::getOrder( ) const
	{
		return order;
	}

	void Operation::setName(const String& name)
	{
		this->name = name;
	}

	void Operation::setDuration(const Duration& duration)
	{
		this->duration = duration;
	}

	const OperationConstraints& Operation::constraints( ) const
	{
		return operation_constraints;
	}

	OperationConstraints& Operation::constraints( )
	{
		return operation_constraints;
	}

	bool Operation::operator==(const Operation& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Operation::operator!=(const Operation& rhs) const
	{
		return !(*this == rhs);
	}
}
