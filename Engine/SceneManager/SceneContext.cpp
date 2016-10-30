#include "SceneContext.h"

#include "Operation.h"
#include "Order.h"
#include "Location.h"
#include "Vehicle.h"
#include "Performer.h"
#include "Depot.h"
#include "Attribute.h"
#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	SceneContext::SceneContext(std::size_t id):
	id(id)
	{}
	
	std::size_t SceneContext::getId() const
	{
		return id;
	}
	
	SceneConstraints& SceneContext::constraints()
	{
		return scene_constraints;
	}
	
	const SceneConstraints& SceneContext::constraints() const
	{
		return scene_constraints;
	}
	
	Operation& SceneContext::createFreeOperation(const Location &location)
	{
		Operation& operation = *operations_factory.createObject(location, None);
        free_operations.push_back(operation);
        return operation;
	}
	
	Location& SceneContext::createLocation(const Site& site)
	{
		Location& location = *locations_factory.createObject(site);
		locations.push_back(location);
		return location;
	}
	
	Order& SceneContext::createOrder(Optional<const Depot&> depot)
	{
		Order::Context context{operations_factory};
		
		Order& order = *orders_factory.createObject(context, depot);
        orders.push_back(order);
        return order;
	}
	
	Performer& SceneContext::createPerformer(Optional<const Depot&> depot)
	{
		Performer& performer = *performers_factory.createObject(depot);
		performers.push_back(performer);
		return performer;
	}
	
	Vehicle& SceneContext::createVehicle(Optional<const Depot&> depot)
	{
		Vehicle& vehicle = *vehicles_factory.createObject(depot);
		vehicles.push_back(vehicle);
		return vehicle;
	}
	
	Depot& SceneContext::createDepot(const Location &location)
	{
		Depot& depot = *depots_factory.createObject(location);
		depots.push_back(depot);
		return depot;
	}
	
	const SceneContext::OperationsList& SceneContext::getFreeOperations() const
	{
		return free_operations;
	}
	
	const SceneContext::LocationsList& SceneContext::getLocations() const
	{
		return locations;
	}

	const SceneContext::OrdersList& SceneContext::getOrders() const
	{
		return orders;
	}
	
	const SceneContext::PerformersList& SceneContext::getPerformers() const
	{
		return performers;
	}
	
	const SceneContext::VehiclesList& SceneContext::getVehicles() const
	{
		return vehicles;
	}
	
	const Attribute& SceneContext::createAttribute(const String& name)
	{
		auto iter = attributes.find(name);
		if (iter != attributes.end()) return iter->second;

		Attribute& attribute = *attributes_factory.createObject(name);
		attributes.emplace(name, attribute);

		return attribute;
	}
	
	const SceneContext::AttributesMap& SceneContext::getAttributes() const
	{
		return attributes;
	}

	
	const SceneContext::DepotsList& SceneContext::getDepots() const
	{
		return depots;
	}
	
	SceneContext::~SceneContext()
	{
        for(const Order& order : orders)
        {
            orders_factory.destroyObject(const_cast<Order*>(&order));
        }

        for(const Operation& operation : free_operations)
        {
            operations_factory.destroyObject(const_cast<Operation*>(&operation));
        }

        for(const Vehicle& vehicle : vehicles)
        {
            vehicles_factory.destroyObject(const_cast<Vehicle*>(&vehicle));
        }

        for(const Performer& performer : performers)
        {
            performers_factory.destroyObject(const_cast<Performer*>(&performer));
        }
        
        for(const Location& location : locations)
        {
            locations_factory.destroyObject(const_cast<Location*>(&location));
        }
        
        for(const Depot& depot : depots)
        {
            depots_factory.destroyObject(const_cast<Depot*>(&depot));
        }
        
        for (const auto &iter : attributes)
		{
			attributes_factory.destroyObject(const_cast<Attribute*>(&iter.second.get()));
		}
	}
	
	bool SceneContext::operator==(const SceneContext& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}
	
	bool SceneContext::operator!=(const SceneContext& rhs) const
	{
		return !(*this == rhs);
	}

}