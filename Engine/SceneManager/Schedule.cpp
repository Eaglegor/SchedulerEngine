#include <assert.h>
#include <algorithm>
#include "Schedule.h"
#include "Run.h"
#include "Extensions/RunVehicleBinder.h"

namespace Scheduler {

    Schedule::Schedule(size_t id, const Performer* performer) :
            id(id),
            performer(performer),
			runs_factory(nullptr),
			shift_start_location_specified(false),
			shift_end_location_specified(false),
			schedule_actualizer(this),
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
		if(!runs_factory) return nullptr;
		if(!(index >= 0 && index <= runs.size())) return nullptr;

		Run* r = runs_factory->createObject(from, to, this);

		r->setStopsFactory(stops_factory);
		r->setScheduleActualizer(&schedule_actualizer);

		if(run_vehicle_binder) run_vehicle_binder->bindVehicle(r);

		runs.insert(runs.begin() + index, r);

		schedule_actualizer.onRunAdded(r, index);

		return r;
	}

	void Schedule::destroyRun(Run *run, size_t hint) {
		auto iter = std::find(runs.begin(), runs.end(), run);
		if(iter == runs.end()) return;
		runs.erase(iter);
		assert(runs_factory);
		runs_factory->destroyObject(run);

		schedule_actualizer.onRunRemoved();
	}

	void Schedule::destroyRun(size_t index) {
		Run* r = runs[index];
		runs.erase(runs.begin() + index);
		assert(runs_factory);
		runs_factory->destroyObject(r);

		schedule_actualizer.onRunRemoved();
	}

	Schedule::~Schedule() {
		assert(runs_factory);

		for(Run* r : runs)
		{
			runs_factory->destroyObject(r);
		}
	}

	const Location& Schedule::getShiftStartLocation() const {
		return shift_start_location;
	}

	void Schedule::setShiftStartLocation(const Location &shift_start_location) {
		this->shift_start_location = shift_start_location;
		shift_start_location_specified = true;
	}

	const Location& Schedule::getDepotLocation() const {
		return depot_location;
	}

	void Schedule::setDepotLocation(const Location &depot_location) {
		this->depot_location = depot_location;

		if(!shift_start_location_specified) shift_start_location = depot_location;
		if(!shift_end_location_specified) shift_end_location = depot_location;
	}

	const Location& Schedule::getShiftEndLocation() const {
		return shift_end_location;
	}

	void Schedule::setShiftEndLocation(const Location &shift_end_location) {
		this->shift_end_location = shift_end_location;
		shift_end_location_specified = true;
	}

	bool Schedule::hasSpecificStartLocation() const
	{
		return shift_start_location_specified;
	}

	bool Schedule::hasSpecificEndLocation() const
	{
		return shift_end_location_specified;
	}

	void Schedule::setStopsFactory(SceneObjectsFactory<WorkStop> *factory)
	{
		this->stops_factory = factory;
	}

	ScheduleActualizer *Schedule::getScheduleActualizer() {
		return &schedule_actualizer;
	}

	const TimeWindow& Schedule::getShift() const
	{
		return shift;
	}

	void Schedule::setShift(const TimeWindow &shift)
	{
		this->shift = shift;
	}

	void Schedule::setRunVehicleBinder(RunVehicleBinder *run_vehicle_binder) {
		this->run_vehicle_binder = run_vehicle_binder;
	}
}