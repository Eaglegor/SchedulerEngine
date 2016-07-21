#include "Schedule.h"
#include <assert.h>
#include <algorithm>
#include "Run.h"
#include "WorkStop.h"
#include "Vehicle.h"
#include "Extensions/RunVehicleBinder.h"
#include "ScheduleStateUtils.h"
#include "ScheduleValidationModel.h"
#include "Extensions/ScheduleValidationAlgorithm.h"

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

	Schedule::Schedule(size_t id, const Schedule* rhs):
		id(id),
		name(rhs->name),
		schedule_actualization_model(rhs->schedule_actualization_model),
		schedule_validation_model(rhs->schedule_validation_model),
		performer(rhs->performer),
		scene(nullptr),
		runs_factory(rhs->runs_factory),
		stops_factory(rhs->stops_factory),
		run_vehicle_binder(rhs->run_vehicle_binder),
		depot_location(rhs->depot_location),
		shift(rhs->shift)
	{
		ScheduleStateUtils::copyState(rhs, this);
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

	const ImmutableVector<Run*>& Schedule::getRuns() const {
		return runs;
	}

	ImmutableVector<Run*>& Schedule::getRuns() {
		return runs;
	}

	Run *Schedule::createRun(const Location &from, const Location &to) {
		return createRun(from, to, runs.size());
	}

	void Schedule::setRunsFactory(SceneObjectsFactory<Run> *factory) {
		this->runs_factory = factory;
	}

	Run *Schedule::createRun(const Location &from, const Location &to, size_t index) {
		assert(runs_factory);
		assert(index >= 0 && index <= runs.size());
		if (!runs_factory) return nullptr;
		if (!(index >= 0 && index <= runs.size())) return nullptr;

		Run* r = runs_factory->createObject(from, to, this);

		r->setStopsFactory(stops_factory);
		r->setScheduleActualizationModel(schedule_actualization_model);
		r->setScheduleValidationModel(schedule_validation_model);

		if(index > 0)
		{
			runs[index - 1]->getEndStop()->invalidateRoute();
		}

		invalidateArrivalTimes();

		if (run_vehicle_binder) run_vehicle_binder->bindVehicle(r);

		Stop* prev_stop = nullptr;
		Stop* next_stop = nullptr;

		if (index > 0)
		{
			prev_stop = runs[index - 1]->getEndStop();
		}
		if (index < runs.size())
		{
			next_stop = runs[index]->getStartStop();
		}

		if (prev_stop)
		{
			prev_stop->setNextStop(r->getStartStop());
			r->getStartStop()->setPrevStop(prev_stop);
		}

		if (next_stop)
		{
			r->getEndStop()->setNextStop(next_stop);
			next_stop->setPrevStop(r->getEndStop());
		}

		runs.insert(runs.begin() + index, r);

		return r;
	}

	void Schedule::destroyRun(Run *run, size_t hint) {
		auto iter = std::find(runs.begin(), runs.end(), run);
		if(iter == runs.end()) return;

		destroyRun(std::distance(runs.begin(), iter));
	}

	void Schedule::destroyRun(size_t index) {
		Run* r = runs[index];

		Stop* prev_stop = r->getStartStop()->getPrevStop();
		Stop* next_stop = r->getEndStop()->getNextStop();

		if (prev_stop) prev_stop->setNextStop(next_stop);
		if (next_stop) next_stop->setPrevStop(prev_stop);

		if(index > 0)
		{
			runs[index - 1]->getEndStop()->invalidateRoute();
		}
		runs.erase(runs.begin() + index);
		assert(runs_factory);
		runs_factory->destroyObject(r);

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
}
