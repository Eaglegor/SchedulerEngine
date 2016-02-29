#pragma once

#include <string>
#include <Engine/Concepts/Basic/Location.h>
#include "Performer.h"
#include <Utils/Collections/ImmutableVector.h>
#include <Engine/SceneManager/ScheduleActualization/ScheduleActualizer.h>
#include "ConceptFactory.h"

namespace Scheduler
{
	class Run;
	class Stop;
	class RoutingService;
	class LoggingService;

    class Schedule
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

		void setRunsFactory(ConceptFactory<Run> *factory);
		void setStopsFactory(ConceptFactory<Stop> *factory);

		void setRoutingService(RoutingService *routing_service);

		const Location& getShiftStartLocation() const;

		void setShiftStartLocation(const Location &shift_start_location);

		const Location& getDepotLocation() const;

		void setDepotLocation(const Location &depot_location);

		const Location& getShiftEndLocation() const;

		void setShiftEndLocation(const Location &shift_end_location);

		bool hasSpecificStartLocation() const;

		bool hasSpecificEndLocation() const;

		void setScheduleActualizer(ScheduleActualizer* actualizer);
		ScheduleActualizer* getScheduleActualizer();

	private:
        size_t id;
        std::string name;

        const Performer* performer;

		std::vector<Run*> runs;

		ConceptFactory<Run> *runs_factory;
		ConceptFactory<Stop> *stops_factory;

		bool shift_start_location_specified;
		Location shift_start_location;

		Location depot_location;

		bool shift_end_location_specified;
		Location shift_end_location;

		RoutingService* routing_service;
		ScheduleActualizer* schedule_actualizer;
    };
}
