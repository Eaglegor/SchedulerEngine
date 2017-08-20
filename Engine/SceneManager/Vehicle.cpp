#include "Vehicle.h"

namespace Scheduler
{
	Vehicle::Vehicle(std::size_t id)
	    : id(id)
	{
	}

	std::size_t Vehicle::getId( ) const
	{
		return id;
	}

	const String& Vehicle::getName( ) const
	{
		return name;
	}

	const RoutingProfile& Vehicle::getRoutingProfile( ) const
	{
		return routing_profile;
	}

	const Cost& Vehicle::getDurationUnitCost( ) const
	{
		return duration_unit_cost;
	}

	const Cost& Vehicle::getDistanceUnitCost( ) const
	{
		return distance_unit_cost;
	}

	const Cost& Vehicle::getActivationCost( ) const
	{
		return activation_cost;
	}

	void Vehicle::setName(const String& name)
	{
		this->name = name;
	}

	void Vehicle::setRoutingProfile(const RoutingProfile& routing_profile)
	{
		this->routing_profile = routing_profile;
	}

	void Vehicle::setDurationUnitCost(const Cost& cost)
	{
		this->duration_unit_cost = cost;
	}

	void Vehicle::setDistanceUnitCost(const Cost& cost)
	{
		this->distance_unit_cost = cost;
	}

	void Vehicle::setActivationCost(const Cost& cost)
	{
		this->activation_cost = cost;
	}

	const Vehicle::AttributesSet& Vehicle::getAttributes( ) const
	{
		return attributes;
	}

	void Vehicle::addAttribute(const Attribute& attribute)
	{
		attributes.emplace(attribute);
	}

	void Vehicle::removeAttribute(const Attribute& attribute)
	{
		attributes.erase(attribute);
	}

	void Vehicle::clearAttributes( )
	{
		attributes.clear( );
	}

	const VehicleConstraints& Vehicle::constraints( ) const
	{
		return vehicle_constraints;
	}

	VehicleConstraints& Vehicle::constraints( )
	{
		return vehicle_constraints;
	}

	bool Vehicle::operator==(const Vehicle& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Vehicle::operator!=(const Vehicle& rhs) const
	{
		return !(*this == rhs);
	}
}
