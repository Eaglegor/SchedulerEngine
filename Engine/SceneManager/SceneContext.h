#pragma once

namespace Scheduler
{
	class Operation;
	class Order;
	class Performer;
	class Vehicle;
	class Attribute;
	class Location;
	class Site;
	
	class SCENEMANAGER_EXPORT SceneContext
	{
		public:
			SceneContext(size_t id);
			~SceneContext();
			
			size_t getId() const;
			
			const ImmutableVector<Operation*>& getFreeOperations() const;
			const ImmutableVector<Order*>& getOrders() const;
			const ImmutableVector<Performer*>& getPerformers() const;
			const ImmutableVector<Vehicle*>& getVehicles() const;
			const ImmutableVector<Location*>& getLocations() const;
			
			ImmutableVector<Operation*>& getFreeOperations();
			ImmutableVector<Order*>& getOrders();
			ImmutableVector<Performer*>& getPerformers();
			ImmutableVector<Vehicle*>& getVehicles();
			ImmutableVector<Location*>& getLocations();
			
			Operation* createFreeOperation();
			Order* createOrder();
			Performer* createPerformer();
			Vehicle* createVehicle();
			Location* createLocation(const Site& site);
			
			const SceneConstraints& constraints() const;
			SceneConstraints& constraints();
			
		private:
			size_t id;

			std::vector<Operation*> free_operations;
			std::vector<Order*> orders;
			std::vector<Location*> locations;

			std::vector<Performer*> performers;
			std::vector<Vehicle*> vehicles;

			std::unordered_map<std::string, Attribute*> attributes;
			
			SceneObjectsFactory<Operation> operations_factory;
			SceneObjectsFactory<Order> orders_factory;
			SceneObjectsFactory<Vehicle> vehicles_factory;
			SceneObjectsFactory<Performer> performers_factory;
			SceneObjectsFactory<Attribute> attributes_factory;
			SceneObjectsFactory<Location> locations_factory;
			
			SceneConstraints scene_constraints;
	};
}