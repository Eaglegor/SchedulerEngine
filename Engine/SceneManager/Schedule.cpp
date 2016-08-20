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

namespace Scheduler {

    Schedule::Schedule(size_t id, const Performer* performer) :
            id(id),
            performer(performer),
			scene(nullptr),
			runs_factory(nullptr),
			stops_factory(nullptr),
			schedule_actualization_model(nullptr),
			schedule_validation_model(nullptr),
			run_vehicle_binder(nullptr)
	{
    }

    size_t Schedule::getId() const {
        return id;
    }

    const char* Schedule::getName() const {
        return name.c_str();
    }

    const Performer* Schedule::getPerformer() const {
        return performer;
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

	Schedule::RunsList::iterator Schedule::createRun(RunsList::const_iterator pos, const Scheduler::Location& from, const Scheduler::Location& to) {
		assert(runs_factory);

		Run* r = runs_factory->createObject(from, to, this, stops, pos == runs.end() ? stops.end() : (*pos)->getStops().begin());

		r->setStopsFactory(stops_factory);
		r->setScheduleActualizationModel(schedule_actualization_model);
		r->setScheduleValidationModel(schedule_validation_model);

		invalidateArrivalTimes();

		if (run_vehicle_binder) run_vehicle_binder->bindVehicle(r);

		return runs.insert(pos, r);
	}

	void Schedule::destroyRun(RunsList::iterator pos) {

		assert(runs_factory);
		runs_factory->destroyObject(*pos);

		runs.erase(pos);
		
		invalidateArrivalTimes();
	}

	Schedule::~Schedule() {
		clear();
	}

	const Location& Schedule::getDepotLocation() const {
		return depot_location;
	}

	void Schedule::setDepotLocation(const Location &depot_location) {
		this->depot_location = depot_location;

		invalidateArrivalTimes();
	}

    bool Schedule::isValid() const
    {
		if (schedule_validation_model == nullptr || schedule_validation_model->getScheduleValidationAlgorithm() == nullptr) return true;
		return schedule_validation_model->getScheduleValidationAlgorithm()->isValid(this);
    }

	void Schedule::setActualizationModel(ScheduleActualizationModel* model)
	{
		this->schedule_actualization_model = model;
		for(Run* r : runs)
		{
			r->setScheduleActualizationModel(model);
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
		invalidateArrivalTimes();
	}

	void Schedule::clear()
	{
		assert(runs_factory);

		for (Run* r : runs)
		{
			runs_factory->destroyObject(r);
		}

		runs.clear();
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
	}

	void Schedule::invalidateArrivalTimes()
	{
		for (Run *run : runs)
		{
			run->invalidateArrivalTimes();
		}
	}
	
    ScheduleValidationModel* Schedule::getValidationModel() const{return schedule_validation_model;}
    ScheduleActualizationModel* Schedule::getActualizationModel() const{return schedule_actualization_model;}
    
	const Schedule::StopsList& Schedule::getStops() const
	{
		return stops;
	}

}
