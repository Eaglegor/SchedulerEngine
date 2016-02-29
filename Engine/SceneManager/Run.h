#pragma once

#include <Engine/Concepts/Basic/Location.h>
#include <Utils/Collections/ImmutableVector.h>
#include "ConceptFactory.h"
#include "Operation.h"

namespace Scheduler
{
	class Schedule;
	class Vehicle;
	class Stop;
	class RoutingService;
	class RoutingProfile;

	class Run
	{
	public:
		Run(size_t id, const Location& start_location, const Location& end_location, Schedule* schedule);
		~Run();

		size_t getId();

		const Schedule* getSchedule() const;
		Schedule* getSchedule();

		const Vehicle* getVehicle() const;

		void setVehicle(const Vehicle *vehicle);

		const Stop* getStartStop() const;
		Stop* getStartStop();

		const ImmutableVector<Stop*>& getWorkStops() const;
		ImmutableVector<Stop*>& getWorkStops();

		const Stop* getEndStop() const;
		Stop* getEndStop();

		const Location& getStartLocation() const;
		const Location& getEndLocation() const;

		Stop* allocateStartOperation(const Operation *operation);
		Stop* allocateWorkOperation(const Operation *operation, size_t index);
		Stop* allocateEndOperation(const Operation *operation);

		void unallocateStartOperation(const Operation *operation);
		void unallocateWorkOperation(const Operation *operation, size_t hint = 0);
		void unallocateWorkOperationAt(size_t index);
		void unallocateEndOperation(const Operation *operation);

		void setStopsFactory(ConceptFactory<Stop> *factory);
		void setRoutingService(RoutingService *routing_service);

	private:
		size_t id;
		Schedule* schedule;
		const Vehicle* vehicle;

		Location start_location;
		Location end_location;

		Stop* start_stop;
		std::vector<Stop*> work_stops;
		Stop* end_stop;

		ConceptFactory<Stop> *stops_factory;
		RoutingService *routing_service;

	private:
		void recalculateRoute(Stop *from, Stop *to);
		void recalculateRoutes();
		void recalculateWorkStopRoutes(size_t index);
	};
}