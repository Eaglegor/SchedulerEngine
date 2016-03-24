#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include <Engine/Engine/Services/RoutingService.h>
#include "Views/RunStopsView.h"
#include "ScheduleActualizer.h"

#include <iostream>

namespace Scheduler {

    Run::Run(size_t id, const Location &start_location, const Location &end_location, Schedule *schedule) :
            id(id),
            start_location(start_location),
            end_location(end_location),
            schedule(schedule),
            stops_factory(nullptr),
            start_stop(nullptr),
            end_stop(nullptr),
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

    const Stop *Run::getStartStop() const {
        return start_stop;
    }

    Stop *Run::getStartStop() {
        return start_stop;
    }

    const ImmutableVector<Stop*>& Run::getWorkStops() const {
        return work_stops;
    }

    ImmutableVector<Stop *>& Run::getWorkStops() {
        return work_stops;
    }

    const Stop *Run::getEndStop() const {
        return end_stop;
    }

    Stop *Run::getEndStop() {
        return end_stop;
    }

    const Location &Run::getStartLocation() const {
        return start_location;
    }

    const Location &Run::getEndLocation() const {
        return end_location;
    }

    Stop *Run::allocateStartOperation(const Operation *operation) {
        start_stop->addOperation(operation);
        return start_stop;
    }

    Stop *Run::allocateWorkOperation(const Operation *operation) {
        return allocateWorkOperation(operation, work_stops.size());
    }

    Stop *Run::allocateWorkOperation(const Operation *operation, size_t index) {
        assert(stops_factory);
        assert(index >= 0 && index <= work_stops.size());
        if (!stops_factory) return nullptr;

		Stop *stop = createStop(operation);

        work_stops.insert(work_stops.begin() + index, stop);

        schedule_actualizer->onStopAdded(this, stop, index);

        invalidateWorkStopRoutes(index);

        return stop;
    }

    Stop *Run::allocateEndOperation(const Operation *operation) {
        end_stop->addOperation(operation);
        return end_stop;
    }

    void Run::unallocateStartOperation(const Operation *operation) {
        assert(start_stop);
        start_stop->removeOperation(operation);
    }

    void Run::unallocateWorkOperation(const Operation *operation, size_t hint) {
        for (size_t i = hint; i < work_stops.size(); ++i) {
            if (work_stops[i]->containsOperation(operation)) {
				unallocateWorkOperationAt(i);
                return;
            }
        }
        for (size_t i = 0; i < hint; ++i) {
            if (work_stops[i]->containsOperation(operation)) {
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

        if(work_stops.empty()) start_stop->invalidateRoute();
        else invalidateWorkStopRoutes(index > 0 ? index - 1 : 0);

        schedule_actualizer->onStopRemoved(this);
    }

    void Run::unallocateEndOperation(const Operation *operation) {
        assert(end_stop);

        end_stop->removeOperation(operation);
    }

	Stop * Run::replaceWorkOperation(const Operation * old_operation, const Operation * new_operation, size_t hint)
	{
		for (size_t i = hint; i < work_stops.size(); ++i)
		{
			Stop* stop = work_stops[i];
			if (*stop->getOperations().begin() == old_operation)
			{
				return replaceWorkOperationAt(i, new_operation);
			}
		}
		for (size_t i = 0; i < hint; ++i)
		{
			Stop* stop = work_stops[i];
			if (*stop->getOperations().begin() == old_operation)
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

		Stop* stop = createStop(new_operation);

		work_stops[index] = stop;

		schedule_actualizer->onStopReplaced(this, stop, index);

		invalidateWorkStopRoutes(index);

		return stop;
	}

    void Run::setStopsFactory(SceneObjectsFactory<Stop> *factory) {
        this->stops_factory = factory;

        start_stop = stops_factory->createObject(start_location, this);
        end_stop = stops_factory->createObject(end_location, this);

		if(schedule_actualizer) start_stop->setScheduleActualizer(schedule_actualizer);
		if(schedule_actualizer) end_stop->setScheduleActualizer(schedule_actualizer);

		start_stop->invalidateRoute();
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
			start_stop->invalidateRoute();
        }

        if (index < work_stops.size() - 1) {
			work_stops[index]->invalidateRoute();
        } else {
			work_stops[index]->invalidateRoute();
        }
    }

    Run::~Run() {
        assert(stops_factory);

        for(Stop* stop : RunStopsView(this))
        {
            stops_factory->destroyObject(stop);
        }
    }

    void Run::setScheduleActualizer(ScheduleActualizer *actualizer) {
        assert(actualizer);

        this->schedule_actualizer = actualizer;

		if(start_stop) start_stop->setScheduleActualizer(actualizer);
		if(end_stop) end_stop->setScheduleActualizer(actualizer);
    }

	Stop* Run::createStop(const Operation * operation)
	{
		Stop *stop = stops_factory->createObject(operation->getLocation(), this);
		stop->setScheduleActualizer(schedule_actualizer);
		stop->addOperation(operation);
		return stop;
	}
}