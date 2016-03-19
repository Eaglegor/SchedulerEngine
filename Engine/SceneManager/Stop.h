#pragma once

#include <cstddef>
#include <Engine/Utils/Collections/ImmutableUnorderedSet.h>
#include "Operation.h"
#include <Engine/Concepts/Route.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Run;
	class ScheduleActualizer;

	/**
		Class representing a single stop to perform some operation (e.g. delivering order or working at a customer site).
	*/
	class SCENEMANAGER_EXPORT Stop
	{
	public:
		Stop(size_t id, const Location &location, Run *run);

		size_t getId() const;

		const ImmutableUnorderedSet<const Operation*> & getOperations() const;

		void addOperation(const Operation *operation);
		void removeOperation(const Operation *operation);

		bool containsOperation(const Operation *operation) const;

		void setStartTime(const TimePoint& time);
		void setEndTime(const TimePoint& time);

		const TimeWindow& getAllocationTime() const;
		void setAllocationTime(const TimeWindow &time);

		const Duration& getDuration() const;
		void setDuration(const Duration &duration);

		const Route& getNextRoute() const;
		void setNextRoute(const Route &route);

		const Run* getRun() const;
		Run* getRun();

		const Location& getLocation() const;

		// == framework internal ====================================
		void setScheduleActualizer(ScheduleActualizer* actualizer);

	private:
		size_t id;

		TimeWindow allocation_time;
		std::unordered_set<const Operation*> operations;
		Duration duration;
		Route next_route;
		Location location;
		Run* run;

		ScheduleActualizer* schedule_actualizer;
	};
}