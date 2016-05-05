#pragma once

#include <cstddef>
#include <Engine/Concepts/Route.h>
#include <Engine/Concepts/TimeWindow.h>
#include "Actualizable.h"
#include <SceneManager_export.h>

#include "ArrivalTimeActualizer.h"
#include "DurationActualizer.h"
#include "RouteActualizer.h"

namespace Scheduler
{
	class ScheduleActualizationModel;
	class ScheduleValidationModel;
	class Run;
	class Operation;
	class RouteActualizationAlgorithm;
	class ArrivalTimeActualizationAlgorithm;
	class DurationActualizationAlgorithm;

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
		void setScheduleActualizationModel(ScheduleActualizationModel* model);
		void setScheduleValidationModel(ScheduleValidationModel* model);

		void invalidateRoute();
		void invalidateArrivalTime();
		void invalidateDuration();

		void setNextStop(Stop* stop);
		void setPrevStop(Stop* stop);

		virtual void acceptVisitor(StopVisitor* visitor) = 0;

		bool isValid() const;

	private:
		using ActualizableAllocationTime = Actualizable<TimeWindow, ArrivalTimeActualizer>;
		using ActualizableDuration = Actualizable<Duration, DurationActualizer>;
		using ActualizableRoute = Actualizable<Route, RouteActualizer>;

		ActualizableAllocationTime allocation_time;
        ActualizableDuration duration;
		ActualizableRoute next_route;

		Run* run;

		Stop* nextStop;
		Stop* prevStop;

		ScheduleActualizationModel* schedule_actualizaton_model;
		ScheduleValidationModel* schedule_validation_model;
	};
}