#pragma once

#include <cstddef>
#include <unordered_map>

#include "Constraints/Scene/SceneConstraints.h"
#include "SceneObjectsFactory.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/Utils/String.h>

namespace Scheduler
{
	class WorkOperation;
	class DepotOperation;
	class Order;
	class Performer;
	class Vehicle;
	class Attribute;
	class Site;
	class Location;
	class Depot;

	class SceneContext
	{
	private:
		template <typename T>
		using ReferencesVector = std::vector<ReferenceWrapper<T>>;

	public:
		using OrdersList     = ReferencesVector<const Order>;
		using PerformersList = ReferencesVector<const Performer>;
		using VehiclesList   = ReferencesVector<const Vehicle>;
		using LocationsList  = ReferencesVector<const Location>;
		using DepotsList     = ReferencesVector<const Depot>;
		using AttributesMap  = std::unordered_map<std::string, ReferenceWrapper<const Attribute>>;

		explicit SceneContext(std::size_t id);
		~SceneContext( );

		std::size_t getId( ) const;

		const OrdersList& getOrders( ) const;
		const PerformersList& getPerformers( ) const;
		const VehiclesList& getVehicles( ) const;
		const LocationsList& getLocations( ) const;
		const DepotsList& getDepots( ) const;
		const AttributesMap& getAttributes( ) const;

		Order& createOrder(const Location& location);
		Performer& createPerformer( );
		Vehicle& createVehicle( );
		Location& createLocation(const Site& site);
		Depot& createDepot(const Location& location);
		const Attribute& createAttribute(const String& name);

		const SceneConstraints& constraints( ) const;
		SceneConstraints& constraints( );

		bool operator==(const SceneContext& rhs) const;
		bool operator!=(const SceneContext& rhs) const;

	private:
		std::size_t id;

		OrdersList orders;
		LocationsList locations;

		PerformersList performers;
		VehiclesList vehicles;
		DepotsList depots;

		AttributesMap attributes;

		SceneObjectsFactory<WorkOperation> work_operations_factory;
		SceneObjectsFactory<DepotOperation> depot_operations_factory;
		SceneObjectsFactory<Order> orders_factory;
		SceneObjectsFactory<Vehicle> vehicles_factory;
		SceneObjectsFactory<Performer> performers_factory;
		SceneObjectsFactory<Attribute> attributes_factory;
		SceneObjectsFactory<Location> locations_factory;
		SceneObjectsFactory<Depot> depots_factory;

		SceneConstraints scene_constraints;
	};
}
