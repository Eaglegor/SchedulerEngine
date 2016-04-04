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
		Stop(Run *run);
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

		virtual const Location& getLocation() const = 0;

		Stop* getNextStop() const;
		Stop* getPrevStop() const;

		// == framework internal ====================================
		void setScheduleActualizer(ScheduleActualizer* actualizer);

		void invalidateRoute();
		bool hasActualRoute() const;

		void setNextStop(Stop* stop);
		void setPrevStop(Stop* stop);

		virtual void acceptVisitor(StopVisitor* visitor) = 0;

	protected:
		ScheduleActualizer* schedule_actualizer;

	private:
		TimeWindow allocation_time;
		Duration duration;
		Route next_route;
		Run* run;

		Stop* nextStop;
		Stop* prevStop;

		bool has_actual_route;
	};
}