#pragma once

#include <cstddef>
#include <Engine/Concepts/Route.h>
#include <Engine/Concepts/TimeWindow.h>

#include <SceneManager_export.h>

namespace Scheduler
{
	class Run;
	class ScheduleActualizer;
	class RouteActualizer;
	class Operation;

	class StopVisitor;

	/**
		Class representing a single stop to perform some operation (e.g. delivering order or working at a customer site).
	*/
	class SCENEMANAGER_EXPORT Stop
	{
	public:
		Stop(const Location &location, Run *run);
		virtual ~Stop() {}

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

		void invalidateRoute();
		bool hasActualRoute() const;

		virtual void acceptVisitor(StopVisitor* visitor) = 0;

	protected:
		ScheduleActualizer* schedule_actualizer;

	private:
		TimeWindow allocation_time;
		Duration duration;
		Route next_route;
		Location location;
		Run* run;

		bool has_actual_route;
	};
}