#pragma once

#include <Engine/Concepts/Location.h>
#include <Engine/Utils/Collections/ImmutableVector.h>
#include "SceneObjectsFactory.h"
#include "Operation.h"

#include <SceneManager_export.h>

namespace Scheduler
{
	class Schedule;
	class Vehicle;
	class Stop;
	class RoutingService;
	class RoutingProfile;
	class ScheduleActualizer;

	/**
		Class representing single performer trip which starts and ends in the specified locations.

		Each run must be associated with a vehicle to be correclty processed by the scene manager (otherwise all routes will be zero  because the routing profile is not provided).
		Default vehicle assigner (if was set for the Scene) is populated by the Scene after the run is created.
		User may explicitly override the vehicle set by the scene by this is not recommended.

		Has 3 operation slots (according to the order's specification):
		- start operations: are allocated to the first stop (at start location) - there can be multiple start operations for the first stop
		- work operations: are allocated between the first and last stop - there is only one work operations for a single work stop
		- end operations: are allocated to the last stop (at end location) - there can be multiple end operations for the last stop
	*/
	class SCENEMANAGER_EXPORT Run
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
		Stop* allocateWorkOperation(const Operation *operation);
		Stop* allocateEndOperation(const Operation *operation);

		void unallocateStartOperation(const Operation *operation);
		void unallocateWorkOperation(const Operation *operation, size_t hint = 0);
		void unallocateWorkOperationAt(size_t index);
		void unallocateEndOperation(const Operation *operation);

		Stop* replaceWorkOperation(const Operation *old_operation, const Operation *new_operation, size_t hint = 0);
		Stop* replaceWorkOperationAt(size_t index, const Operation* new_operation);

		// == framework internal ====================================
		void setStopsFactory(SceneObjectsFactory<Stop> *factory);
		void setScheduleActualizer(ScheduleActualizer* actualizer);

	private:
		Stop* createStop(const Operation* operation);

		size_t id;
		Schedule* schedule;
		const Vehicle* vehicle;

		Location start_location;
		Location end_location;

		Stop* start_stop;
		std::vector<Stop*> work_stops;
		Stop* end_stop;

		SceneObjectsFactory<Stop> *stops_factory;

		ScheduleActualizer* schedule_actualizer;

	private:
		void invalidateRoutes();
		void invalidateWorkStopRoutes(size_t index);
	};
}