#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include <Engine/Engine/Services/RoutingService.h>
#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"
#include "Extensions/RunValidationAlgorithm.h"
#include "WorkStop.h"

#include <iostream>

namespace Scheduler {

    Run::Run(size_t id, const Location &start_location, const Location &end_location, Schedule *schedule, LinkedPointersList<Stop*> &stops_list, LinkedPointersList<Stop*>::iterator pos) :
            id(id),
            start_location(start_location),
            end_location(end_location),
            schedule(schedule),
            stops_factory(nullptr),
            start_stop(start_location, this),
            end_stop(end_location, this),
            vehicle(nullptr),
            schedule_actualization_model(nullptr),
			schedule_validation_model(nullptr)
    {
		auto start = stops_list.insert(pos, &start_stop);
		auto end = stops_list.insert(pos, &end_stop);
		stops.reset(new StopsList(stops_list, start, pos));
		raw_work_stops.reset(new StopsSublist(*stops, std::next(stops->begin()), std::next(stops->begin())));
		work_stops.reset(new WorkStopsList(*raw_work_stops));
    }

    size_t Run::getId() const {
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

	const Run::WorkStopsList& Run::getWorkStops() const
	{
		return *work_stops;
	}

    const RunBoundaryStop *Run::getEndStop() const {
        return &end_stop;
    }

	RunBoundaryStop *Run::getEndStop() {
        return &end_stop;
    }

    RunBoundaryStop *Run::allocateStartOperation(const Operation *operation) {
        start_stop.addOperation(operation);
		start_stop.invalidateDuration();
		invalidateArrivalTimes();
        return &start_stop;
    }

    Run::WorkStopsList::iterator Run::createWorkStop(WorkStopsList::iterator pos, const Operation *operation) {
        assert(stops_factory);

		WorkStop *stop = createWorkStop(operation);
		auto iter = work_stops->insert(pos, stop);
				
        invalidateWorkStopRoutes(iter);
		invalidateArrivalTimes();

        return iter;
    }

    RunBoundaryStop *Run::allocateEndOperation(const Operation *operation) {
        end_stop.addOperation(operation);
		end_stop.invalidateDuration();
		invalidateArrivalTimes();
        return &end_stop;
    }

    void Run::unallocateStartOperation(const Operation *operation) {
        start_stop.removeOperation(operation);
		start_stop.invalidateDuration();
		invalidateArrivalTimes();
    }

    Run::WorkStopsList::iterator Run::destroyWorkStop(WorkStopsList::iterator pos) {
        assert(stops_factory);

        stops_factory->destroyObject(*pos);

		auto iter = work_stops->erase(pos);
		
        invalidateWorkStopRoutes(iter);
		invalidateArrivalTimes();
		
		return iter;
    }

    void Run::unallocateEndOperation(const Operation *operation) {
        end_stop.removeOperation(operation);
		end_stop.invalidateDuration();
		invalidateArrivalTimes();
    }

	bool Run::isValid() const
	{
		if (schedule_validation_model == nullptr || schedule_validation_model->getRunValidationAlgorithm() == nullptr) return true;
		return schedule_validation_model->getRunValidationAlgorithm()->isValid(this);
	}

    void Run::setStopsFactory(SceneObjectsFactory<WorkStop> *factory) {
        this->stops_factory = factory;
    }

    void Run::setVehicle(const Vehicle *vehicle) {
        if (vehicle &&
            (this->vehicle == nullptr || vehicle->getRoutingProfile() != this->vehicle->getRoutingProfile())) {
            this->vehicle = vehicle;
            invalidateRoutes();
			invalidateArrivalTimes();
        }
        else {
            this->vehicle = vehicle;
        }
    }

    void Run::invalidateRoutes() {
        if (vehicle == nullptr) return;

		if (!schedule_actualization_model || !schedule_actualization_model->getRouteActualizationAlgorithm()) return;

		for(Stop* stop : *stops)
		{
			stop->invalidateRoute();
		}
    }

    void Run::invalidateWorkStopRoutes(WorkStopsList::iterator iter) {
		(*iter)->invalidateRoute();
		(*std::prev(iter))->invalidateRoute();
    }

    Run::~Run() {
        assert(stops_factory);

        for(WorkStop* stop : *work_stops)
        {
            stops_factory->destroyObject(stop);
        }
        
        stops->clear();
    }

    void Run::setScheduleActualizationModel(ScheduleActualizationModel* model) {
        this->schedule_actualization_model = model;

		start_stop.setScheduleActualizationModel(model);
		end_stop.setScheduleActualizationModel(model);

		for(WorkStop* stop : *work_stops)
		{
			stop->setScheduleActualizationModel(model);
		}
    }

	void Run::setScheduleValidationModel(ScheduleValidationModel * model)
	{
		this->schedule_validation_model = model;

		start_stop.setScheduleValidationModel(model);
		end_stop.setScheduleValidationModel(model);

		for (WorkStop* stop : *work_stops)
		{
			stop->setScheduleValidationModel(model);
		}
	}

	void Run::invalidateArrivalTimes()
	{
		if (!schedule_actualization_model || !schedule_actualization_model->getArrivalTimeActualizationAlgorithm()) return;

		start_stop.invalidateArrivalTime();
		end_stop.invalidateArrivalTime();
		for(WorkStop *stop : *work_stops)
		{
			stop->invalidateArrivalTime();
		}
	}

	WorkStop* Run::createWorkStop(const Operation * operation)
	{
		WorkStop *stop = stops_factory->createObject(this);
		stop->setScheduleActualizationModel(schedule_actualization_model);
		stop->setScheduleValidationModel(schedule_validation_model);
		stop->setOperation(operation);
		invalidateArrivalTimes();
		return stop;
	}
	
	void Run::swapWorkStops(WorkStopsList::iterator first, WorkStopsList::iterator second)
	{
		if(std::next(first) == second)
		{
			work_stops->splice(first, *work_stops, second, std::next(second));
			return;
		} 
		else if(std::next(second) == first)
		{
			work_stops->splice(second, *work_stops, first, std::next(first));
			return;
		}
		WorkStopsList::iterator pos = std::next(second);
		work_stops->splice(first, *work_stops, second, std::next(second));
		work_stops->splice(pos, *work_stops, first, std::next(first));
	}

	void Run::reverseWorkStops(WorkStopsList::iterator first, WorkStopsList::iterator last)
	{
		work_stops->reverse(first, last);
	}

	void Run::spliceOwnWorkStops(WorkStopsList::iterator pos, WorkStopsList::iterator first, WorkStopsList::iterator last)
	{
		work_stops->splice(pos, *work_stops, first, last);
	}

	const Run::StopsList& Run::getStops() const
	{
		return *stops;
	}
}