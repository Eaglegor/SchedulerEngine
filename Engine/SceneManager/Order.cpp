#include <assert.h>
#include "Order.h"
#include "Operation.h"

namespace Scheduler {

    Order::Order(size_t id) :
            id(id),
            start_operation(nullptr),
            work_operation(nullptr),
            end_operation(nullptr),
            operations_factory(nullptr)
    {
    }


    const char* Order::getName() const {
        return name.c_str();
    }

	Operation* Order::getStartOperation(){
		return start_operation;
	}

    const Operation* Order::getStartOperation() const {
        return start_operation;
    }

    Operation* Order::getWorkOperation() {
        return work_operation;
    }

	const Operation* Order::getWorkOperation() const {
		return work_operation;
	}

	Operation* Order::getEndOperation() {
		return end_operation;
	}

    const Operation* Order::getEndOperation() const {
        return end_operation;
    }

	size_t Order::getId() const {
        return id;
    }

    void Order::setName(const char *name) {
        this->name = name;
    }

	const OrderConstraints & Order::constraints() const
	{
		return order_constraints;
	}

	OrderConstraints & Order::constraints()
	{
		return order_constraints;
	}

    void Order::setOperationsFactory(SceneObjectsFactory<Operation> *factory) {
        this->operations_factory = factory;
    }

    Operation *Order::createStartOperation(const Location& location) {
        assert(operations_factory);
        if(!operations_factory) return nullptr;

        if(this->start_operation) operations_factory->destroyObject(this->start_operation);

		Operation* operation = operations_factory->createObject(location);
		operation->setOrder(this);
        this->start_operation = operation;
        return operation;
    }

    Operation *Order::createWorkOperation(const Location& location) {
        assert(operations_factory);
        if(!operations_factory) return nullptr;

		if(this->work_operation) operations_factory->destroyObject(this->work_operation);
		
        work_operation = operations_factory->createObject(location);
		work_operation->setOrder(this);

        return work_operation;
    }

    Operation *Order::createEndOperation(const Location& location) {
        assert(operations_factory);
        if(!operations_factory) return nullptr;

        if(this->end_operation) operations_factory->destroyObject(this->end_operation);

		Operation* operation = operations_factory->createObject(location);
		operation->setOrder(this);
        this->end_operation = operation;
        return operation;
    }

    Order::~Order() {
        if(start_operation) operations_factory->destroyObject(start_operation);
		if(work_operation) operations_factory->destroyObject(work_operation);
        if(end_operation) operations_factory->destroyObject(end_operation);
    }
}