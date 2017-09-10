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

	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Vehicle Routing Problem task definition
	 * 
	 * @details Scene context is a shared immutable part of VRP solution.
	 * It contains orders, resources (performers and vehicles), locations, attributes, depots.
	 * Scene context doesn't contain any assignments of orders to the resources so it doesn't
	 * represent the solution.
	 * 
	 * @sa Scene
	 */
	class SceneContext
	{
	private:
		template <typename T>
		using ReferencesVector = std::vector<ReferenceWrapper<T>>;

	public:
		/**
		 * @brief List of orders
		 */
		using OrdersList     = ReferencesVector<const Order>;

		/**
		 * @brief List of performers
		 */
		using PerformersList = ReferencesVector<const Performer>;

		/**
		 * @brief List of vehicles
		 */
		using VehiclesList   = ReferencesVector<const Vehicle>;

		/**
		 * @brief List of locations
		 */
		using LocationsList  = ReferencesVector<const Location>;

		/**
		 * @brief List of depots
		 */
		using DepotsList     = ReferencesVector<const Depot>;

		/**
		 * @brief Map of the attributes
		 * 
		 * @details Only single attribute is created for every name, 
		 * this mapping is used to find already created attributes with the same name
		 */
		using AttributesMap  = std::unordered_map<std::string, ReferenceWrapper<const Attribute>>;

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of scene context
		 */
		explicit SceneContext(std::size_t id);

		/**
		 * @brief Destructor
		 * 
		 * @details Destroys all data (performers, vehicles, depots etc.)
		 */
		~SceneContext( );

		/**
		 * @brief Returns identifier of scene context
		 * 
		 * @return Identifier of scene context
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns the list of orders
		 * 
		 * @return List of orders
		 */
		const OrdersList& getOrders( ) const;

		/**
		 * @brief Returns the list of performers
		 * 
		 * @return List of performers
		 */
		const PerformersList& getPerformers( ) const;

		/**
		 * @brief Returns the list of vehicles
		 * 
		 * @return List of vehicles
		 */
		const VehiclesList& getVehicles( ) const;

		/**
		 * @brief Returns the list of locations
		 * 
		 * @return List of locations
		 */
		const LocationsList& getLocations( ) const;

		/**
		 * @brief Returns the list of depots
		 * 
		 * @return List of depots
		 */
		const DepotsList& getDepots( ) const;

		/**
		 * @brief Returns the map of attributes
		 * 
		 * @return Map of attributes
		 */
		const AttributesMap& getAttributes( ) const;

		/**
		 * @brief Creates new order
		 * 
		 * @param location Location of work operation of order
		 * 
		 * @return Reference to the created order
		 */
		Order& createOrder(const Location& location);

		/**
		 * @brief Creates new performer
		 * 
		 * @return Reference to the created performer
		 */
		Performer& createPerformer( );

		/**
		 * @brief Creates new vehicle
		 * 
		 * @return Reference to the created vehicle
		 */
		Vehicle& createVehicle( );

		/**
		 * @brief Creates new location
		 * 
		 * @param site Geographic coordinates of the location
		 * 
		 * @return Reference to the created location
		 */
		Location& createLocation(const Site& site);

		/**
		 * @brief Creates new depot
		 * 
		 * @param location Location of the depot
		 * 
		 * @return Reference to the created depot
		 */
		Depot& createDepot(const Location& location);

		/**
		 * @brief Creates new attribute
		 * 
		 * @param name The name of new attribute
		 * 
		 * @details If the attribute with the specified name was already created reference to it
		 * is returned. If no such attribute was defined, it is created.
		 * 
		 * @return Reference to the created attribute of the existing one if it was created 
		 * earlier
		 */
		const Attribute& createAttribute(const String& name);

		/**
		 * @brief Returns reference to the scene constraints
		 * 
		 * @return Reference to the scene constraints
		 */
		const SceneConstraints& constraints( ) const;

		/**
		 * @brief Returns reference to the scene constraints
		 * 
		 * @return Reference to the scene constraints
		 */
		SceneConstraints& constraints( );

		/**
		 * @brief Equality operator
		 * 
		 * @param rhs Another scene context
		 * 
		 * @return true, if rhs is the same object as this, false otherwise
		 */
		bool operator==(const SceneContext& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @param rhs another scene context
		 * 
		 * @return result of !(rhs == *this)
		 */
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
