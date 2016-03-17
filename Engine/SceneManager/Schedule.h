#pragma once

#include <string>
#include <Engine/Concepts/Location.h>
#include "Performer.h"
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "ScheduleActualizer.h"
#include "SceneObjectsFactory.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	class Run;
	class Stop;
	class RoutingService;
	class LoggingService;
	class RunVehicleBinder;

	/**
		Class representing single performer's work shift.
	*/
    class SCENEMANAGER_EXPORT Schedule
    {
    public:
        Schedule(size_t id, const Performer *performer);
		~Schedule();

        size_t getId() const;
        const char* getName() const;
        const Performer* getPerformer() const;

        void setName(const char* name);

		const ImmutableVector<Run*>& getRuns() const;
		ImmutableVector<Run*>& getRuns();

		Run* createRun(const Location &from, const Location &to);
		Run* createRun(const Location &from, const Location &to, size_t index);

		void destroyRun(Run *run, size_t hint = 0);
		void destroyRun(size_t index);

		const Location& getShiftStartLocation() const;

		void setShiftStartLocation(const Location &shift_start_location);

		const Location& getDepotLocation() const;

		void setDepotLocation(const Location &depot_location);

		const Location& getShiftEndLocation() const;

		void setShiftEndLocation(const Location &shift_end_location);

		bool hasSpecificStartLocation() const;

		bool hasSpecificEndLocation() const;

		ScheduleActualizer* getScheduleActualizer();

		const TimeWindow& getShift() const;
		void setShift(const TimeWindow &shift);

		// == framework internal ====================================
		void setRunsFactory(SceneObjectsFactory<Run> *factory);
		void setStopsFactory(SceneObjectsFactory<Stop> *factory);
		void setRoutingService(RoutingService *routing_service);

		void setRunVehicleBinder(RunVehicleBinder *run_vehicle_binder);

	private:
        size_t id;
        std::string name;

        const Performer* performer;

		std::vector<Run*> runs;

		SceneObjectsFactory<Run> *runs_factory;
		SceneObjectsFactory<Stop> *stops_factory;

		bool shift_start_location_specified;
		Location shift_start_location;

		Location depot_location;

		bool shift_end_location_specified;
		Location shift_end_location;

		TimeWindow shift;

		RoutingService* routing_service;
		ScheduleActualizer schedule_actualizer;

		RunVehicleBinder* run_vehicle_binder;
    };
}
