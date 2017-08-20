#include "Order.h"
#include "DepotOperation.h"
#include "WorkOperation.h"
#include <assert.h>

namespace Scheduler
{

	Order::Order(std::size_t id, const Context& context, const Location& work_operation_location)
	    : id(id),
	      context(context),
	      work_operation(*context.work_operations_factory.createObject(work_operation_location, *this))
	{
	}

	const String& Order::getName( ) const
	{
		return name;
	}

	Optional<const DepotOperation&> Order::getStartOperation( ) const
	{
		if(!start_operation) return None;
		return start_operation.get( );
	}

	const WorkOperation& Order::getWorkOperation( ) const
	{
		return work_operation;
	}

	WorkOperation& Order::getWorkOperation( )
	{
		return work_operation;
	}

	Optional<const DepotOperation&> Order::getEndOperation( ) const
	{
		if(!end_operation) return None;
		return end_operation.get( );
	}

	std::size_t Order::getId( ) const
	{
		return id;
	}

	void Order::setName(const String& name)
	{
		this->name = name;
	}

	const OrderConstraints& Order::constraints( ) const
	{
		return order_constraints;
	}

	OrderConstraints& Order::constraints( )
	{
		return order_constraints;
	}

	DepotOperation& Order::createStartOperation( )
	{
		assert(!this->start_operation);
		if(this->start_operation) context.depot_operations_factory.destroyObject(&this->start_operation.get( ));
		start_operation = *context.depot_operations_factory.createObject(*this);
		return start_operation.get( );
	}

	DepotOperation& Order::createEndOperation( )
	{
		assert(!this->end_operation);
		if(this->end_operation) context.depot_operations_factory.destroyObject(&this->end_operation.get( ));
		end_operation = *context.depot_operations_factory.createObject(*this);
		return end_operation.get( );
	}

	bool Order::operator==(const Order& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Order::operator!=(const Order& rhs) const
	{
		return !(*this == rhs);
	}

	const Order::Context& Order::getContext( ) const
	{
		return context;
	}

	Order::~Order( )
	{
		if(start_operation) context.depot_operations_factory.destroyObject(&start_operation.get( ));
		context.work_operations_factory.destroyObject(&work_operation);
		if(end_operation) context.depot_operations_factory.destroyObject(&end_operation.get( ));
	}
}
