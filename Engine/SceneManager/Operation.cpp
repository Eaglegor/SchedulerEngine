#include "Operation.h"

namespace Scheduler
{
    Operation::Operation(size_t id, const Location& location) :
            id(id),
            order(nullptr),
            location(location)
    {
    }

    const char* Operation::getName() const {
        return name.c_str();
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

    const Order* Operation::getOrder() const {
        return order;
    }

    void Operation::setName(const char *name) {
        this->name = name;
    }

    void Operation::setDuration(const Duration &duration) {
        this->duration = duration;
    }

    void Operation::setOrder(const Order *order) {
        this->order = order;
    }

	const OperationConstraints & Operation::constraints() const
	{
		return operations_constraints;
	}

	OperationConstraints & Operation::constraints()
	{
		return operations_constraints;
	}

}
