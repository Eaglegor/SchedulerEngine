#pragma once

#include <cstddef>
#include <unordered_map>

#include "Constraints/Scene/SceneConstraints.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "SceneObjectsFactory.h"

namespace Scheduler
{
	class Operation;
	class Order;
	class Performer;
	class Vehicle;
	class Attribute;
	class Site;
	class Location;
	class Depot;
	
	class SCENEMANAGER_EXPORT SceneContext
	{
		public:
			SceneContext(std::size_t id);
			~SceneContext();
			
			std::size_t getId() const;
			
			const ImmutableVector<Operation*>& getFreeOperations() const;
			const ImmutableVector<Order*>& getOrders() const;
			const ImmutableVector<Performer*>& getPerformers() const;
			const ImmutableVector<Vehicle*>& getVehicles() const;
			const ImmutableVector<Location*>& getLocations() const;
			const ImmutableVector<Depot*>& getDepots() const;
			const Attribute* getAttribute(const char* name) const;
			
			ImmutableVector<Operation*>& getFreeOperations();
			ImmutableVector<Order*>& getOrders();
			ImmutableVector<Performer*>& getPerformers();
			ImmutableVector<Vehicle*>& getVehicles();
			ImmutableVector<Location*>& getLocations();
			ImmutableVector<Depot*>& getDepots();

			Operation* createFreeOperation(const Location &location);
			Order* createOrder();
			Performer* createPerformer();
			Vehicle* createVehicle();
			Location* createLocation(const Site& site);
			Depot* createDepot(const Location &location);
			const Attribute* createAttribute(const char* name);
			
			const SceneConstraints& constraints() const;
			SceneConstraints& constraints();
			
		private:
			size_t id;

			std::vector<Operation*> free_operations;
			std::vector<Order*> orders;
			std::vector<Location*> locations;

			std::vector<Performer*> performers;
			std::vector<Vehicle*> vehicles;
			std::vector<Depot*> depots;

			std::unordered_map<std::string, Attribute*> attributes;
			
			SceneObjectsFactory<Operation> operations_factory;
			SceneObjectsFactory<Order> orders_factory;
			SceneObjectsFactory<Vehicle> vehicles_factory;
			SceneObjectsFactory<Performer> performers_factory;
			SceneObjectsFactory<Attribute> attributes_factory;
			SceneObjectsFactory<Location> locations_factory;
			SceneObjectsFactory<Depot> depots_factory;
			
			SceneConstraints scene_constraints;
	};
}