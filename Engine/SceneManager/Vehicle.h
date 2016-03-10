#pragma once

#include <string>
#include <Engine/Concepts/Basic/RoutingProfile.h>
#include <Engine/Concepts/Basic/TimeWindow.h>
#include <Engine/Concepts/Basic/Cost.h>
#include <Utils/Collections/ImmutableVector.h>
#include <Engine/Concepts/Basic/Capacity.h>
#include <Utils/Collections/ImmutableUnorderedSet.h>
#include "Attribute.h"

#include <SchedulerEngine_export.h>

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
    class SCHEDULERENGINE_EXPORT Vehicle
    {
    public:
		Vehicle(size_t id);

        size_t getId() const;

        const char* getName() const;
        const ImmutableVector<TimeWindow>& getAvailabilityWindows() const;
        const RoutingProfile& getRoutingProfile() const;

		const Cost& getDurationUnitCost() const;
		const Cost& getDistanceUnitCost() const;
		const Cost& getActivationCost() const;

		void setName(const char *name);
		void setRoutingProfile(const RoutingProfile &routing_profile);
		void setAvailabilityWindows(const ImmutableVector<TimeWindow> &windows);

		void setDurationUnitCost(const Cost &cost);
		void setDistanceUnitCost(const Cost &cost);
		void setActivationCost(const Cost &cost);

		const Capacity& getCapacity() const;
		void setCapacity(const Capacity &capacity);

		const ImmutableUnorderedSet <const Attribute *>& getAttributes() const;
		void setAttributes(const ImmutableUnorderedSet<const Attribute *> &attributes);

	private:
        size_t id;
        std::string name;
        std::vector<TimeWindow> availability_windows;
        RoutingProfile routing_profile;

		Cost duration_unit_cost;
		Cost distance_unit_cost;
		Cost activation_cost;

		Capacity capacity;

		std::unordered_set<const Attribute*> attributes;
    };
}
