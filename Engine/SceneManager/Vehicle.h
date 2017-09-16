#pragma once

#include "Attribute.h"
#include "Constraints/Vehicle/VehicleConstraints.h"
#include <Engine/Concepts/Capacity.h>
#include <Engine/Concepts/Cost.h>
#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>
#include <Engine/Utils/String.h>
#include <string>


namespace Scheduler
{
	/**
	 * @ingroup scene_manager
	 * 
	 * @brief Vehicle used to perform the run
	 */
	class Vehicle
	{
	public:
		/**
		 * @brief Attribute of the vehicle
		 * 
		 * @todo Rename to "Feature"
		 */
		using Attribute     = Scheduler::Attribute;

		/**
		 * @brief Set of attributes of the vehicle
		 */
		using AttributesSet = std::unordered_set<ReferenceWrapper<const Attribute>>;

		/**
		 * @brief Constructor
		 * 
		 * @param id Identifier of the vehicle
		 */
		explicit Vehicle(std::size_t id);

		/**
		 * @brief Returns identifier of the vehicle
		 * 
		 * @return Identifier of the vehicle
		 */
		std::size_t getId( ) const;

		/**
		 * @brief Returns name of the vehicle
		 * 
		 * @return Name of the vehicle
		 */
		const String& getName( ) const;

		/**
		 * @brief Sets name of the vehicle
		 * 
		 * @param name Name of the vehicle
		 */
		void setName(const String& name);

		/**
		 * @brief Returns the routing profile associated with the vehicle
		 * 
		 * @return Routing profile associated with the vehicle
		 */
		const RoutingProfile& getRoutingProfile( ) const;

		/**
		 * @brief Sets the routing profile for the vehicle
		 * 
		 * @param routing_profile Routing profile for the vehicle
		 */
		void setRoutingProfile(const RoutingProfile& routing_profile);

		/**
		 * @brief Returns cost of a single duration unit of the vehicle's work
		 * 
		 * @note Duration unit is defined by the Duration class
		 * 
		 * @sa Performer::getDurationUnitCost
		 * 
		 * @return Cost of a single duration unit of vehicle's work
		 */
		const Cost& getDurationUnitCost( ) const;

		/**
		 * @brief Returns cost of a single distance unit of the vehicle's work
		 * 
		 * @note Distance unit is defined by the Distance class
		 * 
		 * @return Cost of a single distance unit of vehicle's work
		 */
		const Cost& getDistanceUnitCost( ) const;

		/**
		 * @brief Returns cost of using vehicle
		 * 
		 * @details This is a fixed value of cost units added if the vehicle is used for the run
		 * 
		 * @sa Performer::getActivationCost
		 * 
		 * @return Cost of using vehicle
		 */
		const Cost& getActivationCost( ) const;

		/**
		 * @brief Sets cost of a single duration unit of the vehicle's work
		 * 
		 * @note Duration unit is defined by the Duration class
		 * 
		 * @param cost Cost of a single duration unit of vehicle's work
		 */
		void setDurationUnitCost(const Cost& cost);

		/**
		 * @brief Sets cost of a single distance unit of the vehicle's work
		 * 
		 * @note Distance unit is defined by the Distance class
		 * 
		 * @param cost Cost of a single distance unit of vehicle's work
		 */
		void setDistanceUnitCost(const Cost& cost);

		/**
		 * @brief Sets cost of using vehicle
		 * 
		 * @details This is a cost of the fact of using vehicle during the day
		 * 
		 * @param cost Cost of using performer's work
		 */
		void setActivationCost(const Cost& cost);

		/**
		 * @brief Returns the attributes set of the vehicle
		 * 
		 * @return Attributes set of the vehicle
		 */
		const AttributesSet& getAttributes( ) const;

		/**
		 * @brief Adds the attribute to the vehicle
		 * 
		 * @param attribute Attribute to add to the vehicle
		 */
		void addAttribute(const Attribute& attribute);

		/**
		 * @brief Removes the attribute from the vehicle
		 * 
		 * @param attribute Attribute to remove from the vehicle
		 */
		void removeAttribute(const Attribute& attribute);

		/**
		 * @brief Removes all attributes from the vehicle
		 */
		void clearAttributes( );

		/**
		 * @brief Returns the vehicle constraints
		 * 
		 * @return Vehicle constraints
		 */
		const VehicleConstraints& constraints( ) const;

		/**
		 * @brief Returns the vehicle constraints
		 * 
		 * @return Vehicle constraints
		 */
		VehicleConstraints& constraints( );

		/**
		 * @brief Equality operator
		 * 
		 * @return @c true, if @c rhs is the same object as @c this, @c false otherwise
		 */
		bool operator==(const Vehicle& rhs) const;

		/**
		 * @brief Inequality operator
		 * 
		 * @return Result of !(rhs == *this)
		 */
		bool operator!=(const Vehicle& rhs) const;

	private:
		std::size_t id;
		String name;

		RoutingProfile routing_profile;

		Cost duration_unit_cost;
		Cost distance_unit_cost;
		Cost activation_cost;

		AttributesSet attributes;

		VehicleConstraints vehicle_constraints;
	};
}
