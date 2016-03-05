#pragma once

#include <cstdint>
#include <string>
#include <Engine/Concepts/Basic/TimeWindow.h>
#include <Engine/Concepts/Basic/Cost.h>

#include <Utils/Collections/ImmutableVector.h>
#include <Utils/Collections/ImmutableUnorderedSet.h>
#include "Attribute.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
    class SCHEDULERENGINE_EXPORT Performer
    {
    public:
		Performer(size_t id);

		size_t getId() const;
		const char* getName() const;
		ImmutableVector<TimeWindow>& getAvailabilityWindows();

		const Cost& getDurationUnitCost() const;
		const Cost& getActivationCost() const;

		void setName(const char* name);
		void setAvailabilityWindows(ImmutableVector<TimeWindow> &time_windows);

		void setDurationUnitCost(const Cost &cost);
		void setActivationCost(const Cost &cost);

		const ImmutableUnorderedSet<const Attribute*>& getSkills() const;
		void setSkills(const ImmutableUnorderedSet<const Attribute*> &skills);

	private:
		size_t id;
		std::string name;
		std::vector<TimeWindow> availability_windows;
		Cost duration_unit_cost;
		Cost activation_cost;

		std::unordered_set<const Attribute*> skills;
	};
}