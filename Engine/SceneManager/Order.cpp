#include <assert.h>
#include "Order.h"
#include "Operation.h"

namespace Scheduler {

    Order::Order(std::size_t id, const Context& context, Optional<const Depot&> depot) :
            id(id),
            context(context),
            depot(depot)
    {
    }

    const String& Order::getName() const {
        return name;
    }

    Optional<const Operation&> Order::getStartOperation() const {
		if(!start_operation) return None;
        return start_operation.get();
    }

	Optional<const Operation&> Order::getWorkOperation() const {
		if(!work_operation) return None;
		return work_operation.get();
	}

	Optional<const Operation&> Order::getEndOperation() const {
		if(!end_operation) return None;
		return end_operation.get();
	}

	std::size_t Order::getId() const {
        return id;
    }

    void Order::setName(const String &name) {
        this->name = name;
    }

	const OrderConstraints& Order::constraints() const
	{
		return order_constraints;
	}

	OrderConstraints& Order::constraints()
	{
		return order_constraints;
	}

    Operation& Order::createStartOperation(const Location& location) {
        if(this->start_operation) context.operations_factory.destroyObject(&this->start_operation.get());
		start_operation = *context.operations_factory.createObject(location, *this);
		return start_operation.get();
    }

    Operation& Order::createWorkOperation(const Location& location) {
		if(this->work_operation) context.operations_factory.destroyObject(&this->work_operation.get());
		work_operation = *context.operations_factory.createObject(location, *this);
        return work_operation.get();
    }

    Operation& Order::createEndOperation(const Location& location) {
        if(this->end_operation) context.operations_factory.destroyObject(&this->end_operation.get());
		end_operation = *context.operations_factory.createObject(location, *this);
		return end_operation.get();
    }
    
	bool Order::operator==(const Order& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}
	
	bool Order::operator!=(const Order& rhs) const
	{
		return !(*this == rhs);
	}

	Optional<const Depot&> Order::getDepot() const
	{
		return depot;
	}

	const Order::Context& Order::getContext() const
	{
		return context;
	}
	
	Order::~Order() {
        if(start_operation) context.operations_factory.destroyObject(&start_operation.get());
		if(work_operation) context.operations_factory.destroyObject(&work_operation.get());
        if(end_operation) context.operations_factory.destroyObject(&end_operation.get());
    }
}