#include <assert.h>
#include "Run.h"
#include "Stop.h"
#include "Vehicle.h"
#include <Engine/Engine/Services/RoutingService.h>
#include "ScheduleActualizationModel.h"
#include "ScheduleValidationModel.h"
#include "Extensions/RunValidationAlgorithm.h"
#include "WorkStop.h"
#include "Schedule.h"
#include "Listeners/StructuralChangesObserver.h"


namespace Scheduler {

    Run::Run(std::size_t id, const Location &start_location, const Location &end_location, Schedule *schedule, LinkedPointersList<Stop*> &stops_list, LinkedPointersList<Stop*>::iterator pos) :
            id(id),
            schedule(schedule),
            stops_factory(nullptr),
            start_stop(start_location, this),
            end_stop(end_location, this),
            vehicle(nullptr),
            schedule_actualization_model(nullptr),
			schedule_validation_model(nullptr),
			arrival_time_actualizer(nullptr),
			structural_changes_observer(nullptr)
    {
		auto start = stops_list.insert(pos, &start_stop);
		auto end = stops_list.insert(pos, &end_stop);
		stops.reset(new StopsList(stops_list, start, pos));
		raw_work_stops.reset(new StopsSublist(*stops, std::next(stops->begin()), std::next(stops->begin())));
		work_stops.reset(new WorkStopsList(*raw_work_stops));
    }

    std::size_t Run::getId() const {
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
		
		structural_changes_observer->afterStartOperationAdded(stops->begin(), operation);
		
        return &start_stop;
    }

    Run::WorkStopsList::iterator Run::createWorkStop(WorkStopsList::iterator pos, const Operation *operation) {
        assert(stops_factory);

		WorkStop *stop = createWorkStop(operation);
		auto iter = work_stops->insert(pos, stop);
				
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);

		structural_changes_observer->afterWorkStopCreated(iter);
		
        return iter;
    }

    RunBoundaryStop *Run::allocateEndOperation(const Operation *operation) {
        end_stop.addOperation(operation);
		
		structural_changes_observer->afterEndOperationAdded(std::prev(stops->end()), operation);
		
        return &end_stop;
    }

    void Run::unallocateStartOperation(const Operation *operation) {
		
		structural_changes_observer->beforeStartOperationRemoved(stops->begin(), operation);
		
        start_stop.removeOperation(operation);
    }

    Run::WorkStopsList::iterator Run::destroyWorkStop(WorkStopsList::iterator pos) {
        assert(stops_factory);

		structural_changes_observer->beforeWorkStopDestroyed(pos);
		
		auto iter = work_stops->erase(pos);
		
        stops_factory->destroyObject(*pos);
		
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);
		
		return iter;
    }

    void Run::unallocateEndOperation(const Operation *operation) {
		
		structural_changes_observer->beforeEndOperationRemoved(std::prev(stops->end()), operation);
		
        end_stop.removeOperation(operation);
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
			if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
        }
        else {
            this->vehicle = vehicle;
        }
    }

    Run::~Run() {
        assert(stops_factory);

		auto iter = work_stops->begin();
		while(iter != work_stops->end())
		{
			auto next = std::next(iter);
			destroyWorkStop(iter);
			iter = next;
		}
        stops->clear();
    }

    void Run::setScheduleActualizationModel(Scheduler::ScheduleActualizationModel* model, Scheduler::ArrivalTimeActualizer* arrival_time_actualizer) {
        this->schedule_actualization_model = model;
		
		this->arrival_time_actualizer = arrival_time_actualizer;
		
		duration_actualizer = model ? DurationActualizer(model->getDurationActualizationAlgorithm(), this) : DurationActualizer();
		
		start_stop.setScheduleActualizationModel(model, arrival_time_actualizer, &duration_actualizer);
		end_stop.setScheduleActualizationModel(model, arrival_time_actualizer, &duration_actualizer);

		for(WorkStop* stop : *work_stops)
		{
			stop->setScheduleActualizationModel(model, arrival_time_actualizer, &duration_actualizer);
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

	WorkStop* Run::createWorkStop(const Operation * operation)
	{
		WorkStop *stop = stops_factory->createObject(this);
		stop->setScheduleActualizationModel(schedule_actualization_model, arrival_time_actualizer, &duration_actualizer);
		stop->setScheduleValidationModel(schedule_validation_model);
		stop->setOperation(operation);
		
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);
		
		return stop;
	}
	
	void Run::swapWorkStops(WorkStopsList::iterator first, WorkStopsList::iterator second)
	{
		if(first == second) return;

		structural_changes_observer->beforeWorkStopsSwapped(first, second);
		
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
		
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);
	}

	void Run::reverseWorkStops(WorkStopsList::iterator first, WorkStopsList::iterator last)
	{
		structural_changes_observer->beforeWorkStopsReversed(first, last);
		
		work_stops->reverse(first, last);
		
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);
	}

	void Run::spliceOwnWorkStops(WorkStopsList::iterator pos, WorkStopsList::iterator first, WorkStopsList::iterator last)
	{
		structural_changes_observer->beforeWorkStopsSpliced(pos, first, last);
		
		work_stops->splice(pos, *work_stops, first, last);
		
		if(arrival_time_actualizer) arrival_time_actualizer->setDirty(true);
		duration_actualizer.setDirty(true);
	}

	const Run::StopsList& Run::getStops() const
	{
		return *stops;
	}
	
	void Run::adjustStopsRange(StopsList::iterator begin, StopsList::iterator end)
	{
		stops->adjustRange(begin, end, stops->size());
	}
	
	void Run::setStructuralChangesObserver(StructuralChangesObserver* observer)
	{
		this->structural_changes_observer = observer;
	}

}