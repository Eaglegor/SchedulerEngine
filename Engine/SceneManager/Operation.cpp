#include "Operation.h"

namespace Scheduler
{
    Operation::Operation(std::size_t id, const Location& location, Optional<const Order&> order) :
            id(id),
            location(location),
            order(order)
    {
    }

    const String& Operation::getName() const {
        return name;
    }

    std::size_t Operation::getId() const {
        return id;
    }

    const Location& Operation::getLocation() const {
        return location;
    }

    const Duration& Operation::getDuration() const {
        return duration;
    }

    Optional<const Order&> Operation::getOrder() const {
        return order;
    }

    void Operation::setName(const String& name) {
        this->name = name;
    }

    void Operation::setDuration(const Duration &duration) {
        this->duration = duration;
    }

	const OperationConstraints & Operation::constraints() const
	{
		return operations_constraints;
	}

	OperationConstraints & Operation::constraints()
	{
		return operations_constraints;
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
