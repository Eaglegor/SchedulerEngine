#pragma once

#include <string>
#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Concepts/Cost.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Concepts/Capacity.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include "Attribute.h"
#include "Constraints/Vehicle/VehicleConstraints.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	/**
		 Class representing a vehicle.

		 Has routing profile which is used by the routing service to generate feasible routes (e.g. heavy trucks are not allowed to drive through the city center).
		 
		 Has 3 cost factors:
		 - cost per duration unit
		 - cost per distance unit (e.g. based on fuel price)
		 - activation cost
	*/
    class SCENEMANAGER_EXPORT Vehicle
    {
    public:
		Vehicle(size_t id);

        size_t getId() const;

        const char* getName() const;
        const RoutingProfile& getRoutingProfile() const;

		const Cost& getDurationUnitCost() const;
		const Cost& getDistanceUnitCost() const;
		const Cost& getActivationCost() const;

		void setName(const char *name);
		void setRoutingProfile(const RoutingProfile &routing_profile);

		void setDurationUnitCost(const Cost &cost);
		void setDistanceUnitCost(const Cost &cost);
		void setActivationCost(const Cost &cost);

		const ImmutableUnorderedSet <const Attribute *>& getAttributes() const;
		void setAttributes(const ImmutableUnorderedSet<const Attribute *> &attributes);

		const VehicleConstraints& constraints() const;
		VehicleConstraints& constraints();

	private:
        size_t id;
        std::string name;

		RoutingProfile routing_profile;

		Cost duration_unit_cost;
		Cost distance_unit_cost;
		Cost activation_cost;

		std::unordered_set<const Attribute*> attributes;

		VehicleConstraints vehicle_constraints;
    };
}
