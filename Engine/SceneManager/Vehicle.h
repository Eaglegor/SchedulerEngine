#pragma once

#include <string>
#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/TimeWindow.h>
#include <Engine/Concepts/Cost.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include <Engine/Concepts/Capacity.h>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include <Engine/Utils/String.h>
#include <Engine/Utils/Optional.h>
#include "Attribute.h"
#include "Constraints/Vehicle/VehicleConstraints.h"
#include <Engine/Utils/ReferenceWrapper.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Depot;
	
    class SCENEMANAGER_EXPORT Vehicle
    {
    public:
		using Attribute = Scheduler::Attribute;
		using AttributesSet = std::unordered_set<ReferenceWrapper<const Attribute>>;
		
		Vehicle(std::size_t id, Optional<const Depot&> depot);

        std::size_t getId() const;

        const String& getName() const;
		void setName(const String& name);
		
        const RoutingProfile& getRoutingProfile() const;
		void setRoutingProfile(const RoutingProfile &routing_profile);

		const Cost& getDurationUnitCost() const;
		const Cost& getDistanceUnitCost() const;
		const Cost& getActivationCost() const;
		
		void setDurationUnitCost(const Cost &cost);
		void setDistanceUnitCost(const Cost &cost);
		void setActivationCost(const Cost &cost);

		const AttributesSet& getAttributes() const;
		void addAttribute(const Attribute& attribute);
		void removeAttribute(const Attribute& attribute);
		void clearAttributes();

		Optional<const Depot&> getDepot() const;
		
		const VehicleConstraints& constraints() const;
		VehicleConstraints& constraints();
		
		bool operator==(const Vehicle& rhs) const;
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
		
		Optional<const Depot&> depot;
    };
}
