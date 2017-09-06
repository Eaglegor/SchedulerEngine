#pragma once

#include <string>

#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/Collections/ImmutableVector.h>

#include "Constraints/Order/OrderConstraints.h"
#include "SceneObjectsFactory.h"

#include <Engine/Utils/Optional.h>
#include <Engine/Utils/String.h>
#include <boost/optional.hpp>



namespace Scheduler
{
	class Operation;
	class DepotOperation;
	class WorkOperation;
	class Attribute;
	class Location;
	class Depot;

	class Order
	{
	public:
		struct Context
		{
			SceneObjectsFactory<WorkOperation>& work_operations_factory;
			SceneObjectsFactory<DepotOperation>& depot_operations_factory;
		};

		Order(std::size_t id, const Context& context, const Location& work_operation_location);
		~Order( );

		std::size_t getId( ) const;

		const String& getName( ) const;
		void setName(const String& name);

		Optional<const DepotOperation&> getStartOperation( ) const;
		const WorkOperation& getWorkOperation( ) const;
		Optional<const DepotOperation&> getEndOperation( ) const;

		DepotOperation& createStartOperation( );
		WorkOperation& getWorkOperation( );
		DepotOperation& createEndOperation( );

		const OrderConstraints& constraints( ) const;
		OrderConstraints& constraints( );

		const Context& getContext( ) const;

		bool operator==(const Order& rhs) const;
		bool operator!=(const Order& rhs) const;

	private:
		std::size_t id;
		Context context;

		String name;

		Optional<DepotOperation&> start_operation;
		WorkOperation& work_operation;
		Optional<DepotOperation&> end_operation;

		OrderConstraints order_constraints;
	};
}
