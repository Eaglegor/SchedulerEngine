#include "Schedule.h"
#include <assert.h>
#include <algorithm>
#include "Run.h"
#include "WorkStop.h"
#include "Vehicle.h"
#include "Extensions/RunVehicleBinder.h"
#include "ScheduleValidationModel.h"
#include "Extensions/ScheduleValidationAlgorithm.h"
#include <iterator>
#include "Listeners/StructuralChangesObserver.h"

namespace Scheduler {

    Schedule::Schedule(size_t id, const Performer& performer) :
            id(id),
            performer(performer),
			scene(nullptr),
			runs_factory(nullptr),
			stops_factory(nullptr),
			schedule_actualization_model(nullptr),
			schedule_validation_model(nullptr),
			run_vehicle_binder(nullptr),
			structural_changes_observer(nullptr)
	{
    }

    size_t Schedule::getId() const {
        return id;
    }

    const char* Schedule::getName() const {
        return name.c_str();
    }

    const Performer* Schedule::getPerformer() const {
        return &performer;
    }

    void Schedule::setName(const char *name) {
        this->name = name;
    }

	const Schedule::RunsList& Schedule::getRuns() const {
		return runs;
	}

	void Schedule::setRunsFactory(SceneObjectsFactory<Run> *factory) {
		this->runs_factory = factory;
	}

	Schedule::RunsList::iterator Schedule::createRun(RunsList::const_iterator pos, const Location& from, const Location& to) {
		assert(runs_factory);

		Run* r = runs_factory->createObject(from, to, this, stops, pos == runs.end() ? stops.end() : (*pos)->getStops().begin());

		r->setStopsFactory(stops_factory);
		r->setScheduleActualizationModel(schedule_actualization_model, &arrival_time_actualizer);
		r->setScheduleValidationModel(schedule_validation_model);
		r->setStructuralChangesObserver(structural_changes_observer);

		arrival_time_actualizer.setDirty(true);

		if (run_vehicle_binder) run_vehicle_binder->bindVehicle(r);

		if(pos != runs.begin()) 
		{
			Run* prev_run = *std::prev(pos);
			prev_run->adjustStopsRange(prev_run->getStops().begin(), r->getStops().begin());
		}
		
		auto iter = runs.insert(pos, r);
		
		structural_changes_observer->afterRunCreated(iter);
		
		return iter;
	}

	Schedule::~Schedule() {
		clear();
	}

    bool Schedule::isValid() const
    {
		if (schedule_validation_model == nullptr || schedule_validation_model->getScheduleValidationAlgorithm() == nullptr) return true;
		return schedule_validation_model->getScheduleValidationAlgorithm()->isValid(this);
    }

	void Schedule::setActualizationModel(ScheduleActualizationModel* model)
	{
		this->schedule_actualization_model = model;
		
		arrival_time_actualizer = model ? ArrivalTimeActualizer(model->getArrivalTimeActualizationAlgorithm(), this) : ArrivalTimeActualizer();
		
		for(Run* r : runs)
		{
			r->setScheduleActualizationModel(model, &arrival_time_actualizer);
		}
	}

	void Schedule::setValidationModel(ScheduleValidationModel * model)
	{
		this->schedule_validation_model = model;
		for (Run* r : runs)
		{
			r->setScheduleValidationModel(model);
		}
	}

	void Schedule::setStopsFactory(SceneObjectsFactory<WorkStop> *factory)
	{
		this->stops_factory = factory;
	}

	const TimeWindow& Schedule::getShift() const
	{
		return shift;
	}

	void Schedule::setShift(const TimeWindow &shift)
	{
		this->shift = shift;
		arrival_time_actualizer.setDirty(true);
	}

	void Schedule::clear()
	{
		assert(runs_factory);

		for (Run* r : runs)
		{
			runs_factory->destroyObject(r);
		}

		runs.clear();
		
		arrival_time_actualizer.setDirty(true);
	}

	Scene* Schedule::getScene()
	{
		return scene;
	}

	const ScheduleConstraints & Schedule::constraints() const
	{
		return schedule_constraints;
	}

	ScheduleConstraints & Schedule::constraints()
	{
		return schedule_constraints;
	}

	const Scene* Schedule::getScene() const
	{
		return scene;
	}

	void Schedule::setScene(Scene* scene)
	{
		this->scene = scene;
	}

	void Schedule::setRunVehicleBinder(RunVehicleBinder *run_vehicle_binder) {
		this->run_vehicle_binder = run_vehicle_binder;
		
		for(Run* r : runs)
		{
			if(run_vehicle_binder) run_vehicle_binder->bindVehicle(r);
			else r->setVehicle(nullptr);
		}
		
		arrival_time_actualizer.setDirty(true);
	}
	
	RunVehicleBinder* Schedule::getRunVehicleBinder() const
	{
		return run_vehicle_binder;
	}
	
    ScheduleValidationModel* Schedule::getValidationModel() const{return schedule_validation_model;}
    ScheduleActualizationModel* Schedule::getActualizationModel() const{return schedule_actualization_model;}
    
	const Schedule::StopsList& Schedule::getStops() const
	{
		return stops;
	}
	
	void Schedule::setStructuralChangesObserver(StructuralChangesObserver* observer)
	{
		this->structural_changes_observer = observer;
	}
	
	Schedule::RunsList::iterator Schedule::detachRun(RunsList::const_iterator pos)
	{
		Run* const r = *pos;
		
		structural_changes_observer->beforeRunDestroyed(pos);
		
		if(pos != runs.begin()) 
		{
			Run* prev_run = *std::prev(pos);
			prev_run->adjustStopsRange(prev_run->getStops().begin(), (*pos)->getStops().end());
		}
		
		auto iter = runs.erase(pos);
		
		r->is_detached = true;
		
		arrival_time_actualizer.setDirty(true);
		
		return iter;
	}
	
	Schedule::RunsList::iterator Schedule::attachRun(RunsList::iterator pos, Run* run)
	{
		assert(run->is_detached && run->schedule == this);
		
		if(pos != runs.begin()) 
		{
			Run* prev_run = *std::prev(pos);
			prev_run->adjustStopsRange(prev_run->getStops().begin(), run->getStops().begin());
		}
		
		auto iter = runs.insert(pos, run);
		run->is_detached = false;
		
		arrival_time_actualizer.setDirty(true);
		structural_changes_observer->afterRunCreated(iter);
		
		return iter;
	}
	
	void Schedule::destroyDetachedRun(Run* run)
	{
		assert(run->is_detached);
		assert(run->schedule == this);
		assert(runs_factory);
		if(run->schedule == this && run->is_detached) runs_factory->destroyObject(run);
	}
	
	Schedule::RunsList::iterator Schedule::destroyRun(RunsList::iterator pos) {
		auto iter = detachRun(pos);
		destroyDetachedRun(*pos);				
		return iter;
	}
}
