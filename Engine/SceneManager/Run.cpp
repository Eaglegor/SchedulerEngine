#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include <Engine/PluginAPI/Services/RoutingService.h>
#include <Engine/SceneManager/Views/RunStopsView.h>

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
            routing_service(nullptr) {
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

    Stop *Run::allocateWorkOperation(const Operation *operation, size_t index) {
        assert(stops_factory);
        assert(index >= 0 && index <= work_stops.size());
        if (!stops_factory) return nullptr;

        Stop *stop = stops_factory->createObject(operation->getLocation(), this);
        stop->addOperation(operation);

        work_stops.insert(work_stops.begin() + index, stop);

        recalculateWorkStopRoutes(index);

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
                stops_factory->destroyObject(work_stops[i]);
                work_stops.erase(work_stops.begin() + i);

                if(work_stops.empty()) recalculateRoute(start_stop, end_stop);
                else recalculateWorkStopRoutes(i > 0 ? i - 1 : 0);

                return;
            }
        }
        for (size_t i = 0; i < hint; ++i) {
            if (work_stops[i]->containsOperation(operation)) {
                assert(stops_factory);
                stops_factory->destroyObject(work_stops[i]);
                work_stops.erase(work_stops.begin() + i);

                if(work_stops.empty()) recalculateRoute(start_stop, end_stop);
                else recalculateWorkStopRoutes(i > 0 ? i - 1 : 0);

                return;
            }
        }
    }

    void Run::unallocateWorkOperationAt(size_t index) {
        assert(stops_factory);
        stops_factory->destroyObject(work_stops[index]);
        work_stops.erase(work_stops.begin() + index);

        if(work_stops.empty()) recalculateRoute(start_stop, end_stop);
        else recalculateWorkStopRoutes(index > 0 ? index - 1 : 0);
    }

    void Run::unallocateEndOperation(const Operation *operation) {
        assert(end_stop);

        end_stop->removeOperation(operation);
    }

    void Run::setStopsFactory(ConceptFactory<Stop> *factory) {
        this->stops_factory = factory;

        start_stop = stops_factory->createObject(start_location, this);
        end_stop = stops_factory->createObject(end_location, this);

        recalculateRoute(start_stop, end_stop);
    }

    void Run::setVehicle(const Vehicle *vehicle) {
        if (vehicle &&
            (this->vehicle == nullptr || vehicle->getRoutingProfile() != this->vehicle->getRoutingProfile())) {
            this->vehicle = vehicle;
            recalculateRoutes();
        }
        else {
            this->vehicle = vehicle;
        }
    }

    void Run::setRoutingService(RoutingService *routing_service) {
        this->routing_service = routing_service;
    }

    void Run::recalculateRoutes() {
        if (vehicle == nullptr) return;

        RunStopsView all_stops(this);

        if (all_stops.empty()) return;

        auto s1 = all_stops.begin();
        auto s2 = all_stops.begin() + 1;

        while (s2 != all_stops.end()) {
            recalculateRoute(*s1, *s2);
            ++s1;
            ++s2;
        }
    }

    void Run::recalculateRoute(Stop *from, Stop *to) {
        assert(routing_service != nullptr);

        if (vehicle == nullptr) return;

        from->setNextRoute(
                routing_service->calculateRoute(from->getLocation(), to->getLocation(), vehicle->getRoutingProfile()));
    }

    void Run::recalculateWorkStopRoutes(size_t index) {
        assert(index >= 0 && index < work_stops.size());

        if (index > 0) {
            recalculateRoute(work_stops[index - 1], work_stops[index]);
        } else {
            recalculateRoute(start_stop, work_stops[index]);
        }

        if (index < work_stops.size() - 1) {
            recalculateRoute(work_stops[index], work_stops[index + 1]);
        } else {
            recalculateRoute(work_stops[index], end_stop);
        }
    }

    Run::~Run() {
        assert(stops_factory);

        for(Stop* stop : RunStopsView(this))
        {
            stops_factory->destroyObject(stop);
        }
    }
}