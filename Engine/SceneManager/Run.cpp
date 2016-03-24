#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include <Engine/Engine/Services/RoutingService.h>
#include "Views/RunStopsView.h"
#include "ScheduleActualizer.h"
#include "WorkStop.h"

#include <iostream>

namespace Scheduler {

    Run::Run(size_t id, const Location &start_location, const Location &end_location, Schedule *schedule) :
            id(id),
            start_location(start_location),
            end_location(end_location),
            schedule(schedule),
            stops_factory(nullptr),
            start_stop(start_location, this),
            end_stop(end_location, this),
            vehicle(nullptr),
            schedule_actualizer(nullptr)
    {
    }

    size_t Run::getId() {
        return id;
    }

    const Schedule *Run::getSchedule() const {
        return schedule;
    }

    Schedule *Run::getSchedule() {
        return schedule;
    }

    const Vehicle *Run::getVehicle() const {
        return vehicle;
    }

    const RunBoundaryStop *Run::getStartStop() const {
        return &start_stop;
    }

    RunBoundaryStop *Run::getStartStop() {
        return &start_stop;
    }

    const ImmutableVector<WorkStop*>& Run::getWorkStops() const {
        return work_stops;
    }

    ImmutableVector<WorkStop *>& Run::getWorkStops() {
        return work_stops;
    }

    const RunBoundaryStop *Run::getEndStop() const {
        return &end_stop;
    }

	RunBoundaryStop *Run::getEndStop() {
        return &end_stop;
    }

    const Location &Run::getStartLocation() const {
        return start_location;
    }

    const Location &Run::getEndLocation() const {
        return end_location;
    }

    RunBoundaryStop *Run::allocateStartOperation(const Operation *operation) {
        start_stop.addOperation(operation);
        return &start_stop;
    }

    WorkStop *Run::allocateWorkOperation(const Operation *operation) {
        return allocateWorkOperation(operation, work_stops.size());
    }

    WorkStop *Run::allocateWorkOperation(const Operation *operation, size_t index) {
        assert(stops_factory);
        assert(index >= 0 && index <= work_stops.size());
        if (!stops_factory) return nullptr;

		WorkStop *stop = createWorkStop(operation);

        work_stops.insert(work_stops.begin() + index, stop);

        schedule_actualizer->onStopAdded(this, stop, index);

        invalidateWorkStopRoutes(index);

        return stop;
    }

    RunBoundaryStop *Run::allocateEndOperation(const Operation *operation) {
        end_stop.addOperation(operation);
        return &end_stop;
    }

    void Run::unallocateStartOperation(const Operation *operation) {
        start_stop.removeOperation(operation);
    }

    void Run::unallocateWorkOperation(const Operation *operation, size_t hint) {
        for (size_t i = hint; i < work_stops.size(); ++i) {
            if (work_stops[i]->getOperation() == operation) {
				unallocateWorkOperationAt(i);
                return;
            }
        }
        for (size_t i = 0; i < hint; ++i) {
            if (work_stops[i]->getOperation() == operation) {
				unallocateWorkOperationAt(i);
				return;
            }
        }
    }

    void Run::unallocateWorkOperationAt(size_t index) {
        assert(stops_factory);
        assert(index >=0 && index < work_stops.size());
        stops_factory->destroyObject(work_stops[index]);
        work_stops.erase(work_stops.begin() + index);

        if(work_stops.empty()) start_stop.invalidateRoute();
        else invalidateWorkStopRoutes(index > 0 ? index - 1 : 0);

        schedule_actualizer->onStopRemoved(this);
    }

    void Run::unallocateEndOperation(const Operation *operation) {
        end_stop.removeOperation(operation);
    }

	Stop * Run::replaceWorkOperation(const Operation * old_operation, const Operation * new_operation, size_t hint)
	{
		for (size_t i = hint; i < work_stops.size(); ++i)
		{
			WorkStop* stop = work_stops[i];
			if (stop->getOperation() == old_operation)
			{
				return replaceWorkOperationAt(i, new_operation);
			}
		}
		for (size_t i = 0; i < hint; ++i)
		{
			WorkStop* stop = work_stops[i];
			if (stop->getOperation() == old_operation)
			{
				return replaceWorkOperationAt(i, new_operation);
			}
		}
		return nullptr;
	}

	Stop * Run::replaceWorkOperationAt(size_t index, const Operation * new_operation)
	{
		assert(stops_factory);
		
		if (!stops_factory) return nullptr;

		stops_factory->destroyObject(work_stops[index]);

		WorkStop* stop = createWorkStop(new_operation);

		work_stops[index] = stop;

		schedule_actualizer->onStopReplaced(this, stop, index);

		invalidateWorkStopRoutes(index);

		return stop;
	}

    void Run::setStopsFactory(SceneObjectsFactory<WorkStop> *factory) {
        this->stops_factory = factory;
    }

    void Run::setVehicle(const Vehicle *vehicle) {
        if (vehicle &&
            (this->vehicle == nullptr || vehicle->getRoutingProfile() != this->vehicle->getRoutingProfile())) {
            this->vehicle = vehicle;
            invalidateRoutes();
        }
        else {
            this->vehicle = vehicle;
        }
        schedule_actualizer->onRunVehicleChanged(this, vehicle);
    }

    void Run::invalidateRoutes() {
        if (vehicle == nullptr) return;

        RunStopsView all_stops(this);

        if (all_stops.empty()) return;

        auto s1 = all_stops.begin();
        auto s2 = all_stops.begin() + 1;

        while (s2 != all_stops.end()) {
            (*s1)->invalidateRoute();
            ++s1;
            ++s2;
        }
    }

    void Run::invalidateWorkStopRoutes(size_t index) {
        assert(index >= 0 && index < work_stops.size());

        if (index > 0) {
			work_stops[index - 1]->invalidateRoute();
        } else {
			start_stop.invalidateRoute();
        }

        if (index < work_stops.size() - 1) {
			work_stops[index]->invalidateRoute();
        } else {
			work_stops[index]->invalidateRoute();
        }
    }

    Run::~Run() {
        assert(stops_factory);

        for(WorkStop* stop : work_stops)
        {
            stops_factory->destroyObject(stop);
        }
    }

    void Run::setScheduleActualizer(ScheduleActualizer *actualizer) {
        assert(actualizer);

        this->schedule_actualizer = actualizer;

		start_stop.setScheduleActualizer(actualizer);
		end_stop.setScheduleActualizer(actualizer);
    }

	WorkStop* Run::createWorkStop(const Operation * operation)
	{
		WorkStop *stop = stops_factory->createObject(operation->getLocation(), this);
		stop->setScheduleActualizer(schedule_actualizer);
		stop->setOperation(operation);
		return stop;
	}
}