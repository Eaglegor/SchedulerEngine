#include "Vehicle.h"

namespace Scheduler
{
	Vehicle::Vehicle(size_t id): id(id) {

	}

	size_t Vehicle::getId() const{
		return id;
	}

	const char* Vehicle::getName() const {
		return name.c_str();
	}

	const ImmutableVector<TimeWindow>& Vehicle::getAvailabilityWindows() const {
		return availability_windows;
	}

	const RoutingProfile& Vehicle::getRoutingProfile() const {
		return routing_profile;
	}

	const Cost &Vehicle::getDurationUnitCost() const {
		return duration_unit_cost;
	}

	const Cost &Vehicle::getDistanceUnitCost() const {
		return distance_unit_cost;
	}

	const Cost &Vehicle::getActivationCost() const {
		return activation_cost;
	}

	void Vehicle::setName(const char *name) {
		this->name = name;
	}

	void Vehicle::setRoutingProfile(const RoutingProfile &routing_profile) {
		this->routing_profile = routing_profile;
	}

	void Vehicle::setAvailabilityWindows(const ImmutableVector<TimeWindow> &windows) {
		this->availability_windows = windows;
	}

	void Vehicle::setDurationUnitCost(const Cost &cost) {
		this->duration_unit_cost = cost;
	}

	void Vehicle::setDistanceUnitCost(const Cost &cost) {
		this->distance_unit_cost = cost;
	}

	void Vehicle::setActivationCost(const Cost &cost) {
		this->activation_cost = cost;
	}


	const Capacity &Vehicle::getCapacity() const {
		return capacity;
	}

	void Vehicle::setCapacity(const Capacity &capacity) {
		this->capacity = capacity;
	}

	const ImmutableUnorderedSet <const Attribute *> &Vehicle::getAttributes() const {
		return attributes;
	}

	void Vehicle::setAttributes(const ImmutableUnorderedSet<const Attribute *> &attributes) {
		this->attributes = attributes;
	}
	const VehicleConstraints & Vehicle::constraints() const
	{
		return vehicle_constraints;
	}
	VehicleConstraints & Vehicle::constraints()
	{
		return vehicle_constraints;
	}
}

