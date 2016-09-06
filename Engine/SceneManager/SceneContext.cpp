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
	
	Operation* SceneContext::createFreeOperation(const Location &location)
	{
		Operation* operation = operations_factory.createObject(location);
        free_operations.push_back(operation);
        return operation;
	}
	
	Location* SceneContext::createLocation(const Site& site)
	{
		Location* location = locations_factory.createObject(site);
		locations.push_back(location);
		return location;
	}
	
	Order* SceneContext::createOrder()
	{
		Order* order = orders_factory.createObject();
		order->setOperationsFactory(&operations_factory);
        orders.push_back(order);
        return order;
	}
	
	Performer* SceneContext::createPerformer()
	{
		Performer* performer = performers_factory.createObject();
		performers.push_back(performer);
		return performer;
	}
	
	Vehicle* SceneContext::createVehicle()
	{
		Vehicle* vehicle= vehicles_factory.createObject();
		vehicles.push_back(vehicle);
		return vehicle;
	}
	
	Depot* SceneContext::createDepot(const Location &location)
	{
		Depot* depot = depots_factory.createObject(location);
		depots.push_back(depot);
		return depot;
	}
	
	Scheduler::ImmutableVector< Operation* >& SceneContext::getFreeOperations()
	{
		return free_operations;
	}
	
	const Scheduler::ImmutableVector< Operation* >& SceneContext::getFreeOperations() const
	{
		return free_operations;
	}
	
	Scheduler::ImmutableVector< Location* >& SceneContext::getLocations()
	{
		return locations;
	}
	
	const Scheduler::ImmutableVector< Location* >& SceneContext::getLocations() const
	{
		return locations;
	}
	
	Scheduler::ImmutableVector< Order* >& SceneContext::getOrders()
	{
		return orders;
	}
	
	const Scheduler::ImmutableVector< Order* >& SceneContext::getOrders() const
	{
		return orders;
	}

	Scheduler::ImmutableVector< Performer* >& SceneContext::getPerformers()
	{
		return performers;
	}
	
	const Scheduler::ImmutableVector< Performer* >& SceneContext::getPerformers() const
	{
		return performers;
	}
	
	Scheduler::ImmutableVector< Vehicle* >& SceneContext::getVehicles()
	{
		return vehicles;
	}
	
	const Scheduler::ImmutableVector< Vehicle* >& SceneContext::getVehicles() const
	{
		return vehicles;
	}
	
	const Attribute* SceneContext::createAttribute(const char* name)
	{
		auto iter = attributes.find(name);
		if (iter != attributes.end()) return iter->second;

		Attribute* attribute = attributes_factory.createObject(name);
		attributes.emplace(name, attribute);

		return attribute;
	}
	
	const Attribute* SceneContext::getAttribute(const char* name) const
	{
		auto iter = attributes.find(name);
		if (iter != attributes.end()) return iter->second;
		
		return nullptr;
	}
	
	Scheduler::ImmutableVector< Depot* >& SceneContext::getDepots()
	{
		return depots;
	}
	
	const Scheduler::ImmutableVector< Depot* >& SceneContext::getDepots() const
	{
		return depots;
	}
	
	SceneContext::~SceneContext()
	{
        for(Order* order : orders)
        {
            orders_factory.destroyObject(order);
        }

        for(Operation* operation : free_operations)
        {
            operations_factory.destroyObject(operation);
        }

        for(Vehicle* vehicle : vehicles)
        {
            vehicles_factory.destroyObject(vehicle);
        }

        for(Performer* performer : performers)
        {
            performers_factory.destroyObject(performer);
        }
        
        for(Location* location : locations)
        {
            locations_factory.destroyObject(location);
        }
        
        for(Depot* depot : depots)
        {
            depots_factory.destroyObject(depot);
        }
        
        for (auto &iter : attributes)
		{
			attributes_factory.destroyObject(iter.second);
		}
	}
}