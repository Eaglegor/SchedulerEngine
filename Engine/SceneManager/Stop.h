#pragma once

#include <cstdint>
#include <Utils/Collections/ImmutableUnorderedSet.h>
#include "Operation.h"
#include <Engine/Concepts/Basic/Route.h>

namespace Scheduler
{
	class Run;

	class Stop
	{
	public:
		Stop(size_t id, const Location &location, Run *run);

		size_t getId() const;

		const ImmutableUnorderedSet<const Operation*> & getOperations() const;

		void addOperation(const Operation *operation);
		void removeOperation(const Operation *operation);

		bool containsOperation(const Operation *operation) const;

		const TimeWindow& getAllocationTime() const;
		void setAllocationTime(const TimeWindow &time);

		const Duration& getDuration() const;
		void setDuration(const Duration &duration);

		const Route& getNextRoute();
		void setNextRoute(const Route &route);

		const Run* getRun() const;
		Run* getRun();

		const Location& getLocation() const;

	private:
		size_t id;

		TimeWindow allocation_time;
		std::unordered_set<const Operation*> operations;
		Duration duration;
		Route next_route;
		Location location;
		Run* run;
	};
}