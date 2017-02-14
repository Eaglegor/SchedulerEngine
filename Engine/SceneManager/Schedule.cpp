#include "Schedule.h"
#include <assert.h>
#include <algorithm>
#include "Run.h"
#include "WorkStop.h"
#include "Vehicle.h"
#include "Algorithms/RunVehicleBinder.h"
#include "ScheduleValidationModel.h"
#include "Algorithms/Validation/ValidationAlgorithm.h"
#include "Algorithms/Validation/Validator.h"
#include <Engine/Utils/ReferenceWrapper.h>
#include <iterator>
#include "Listeners/StructuralChangesObserver.h"

namespace Scheduler {

    Schedule::Schedule(std::size_t id, const Context& context, const Performer& performer, const Scene& scene):
	id(id),
	context(context),
	performer(performer),
	scene(scene),
	arrival_time_actualizer(*this)
	{
    }

    std::size_t Schedule::getId() const 
    {
        return id;
    }

    const String& Schedule::getName() const 
    {
        return name;
    }

    const Performer& Schedule::getPerformer() const 
    {
        return performer;
    }

    void Schedule::setName(const String& name) 
	{
        this->name = name;
    }

	const Schedule::RunsList& Schedule::getRuns() const 
	{
		return runs;
	}

	Schedule::RunIterator Schedule::createRun(ConstRunIterator pos, const Location& from, const Location& to) 
	{
		Run::Context run_context
		{
			context.stops_factory,
			context.structural_changes_observer,
			arrival_time_actualizer
		};
		
		Run& r = *context.runs_factory.createObject(run_context, *this, from, to, stops, pos == runs.end() ? stops.end() : pos->get().getStops().begin());

		arrival_time_actualizer.setDirty(true);

		if (run_vehicle_binder) run_vehicle_binder->bindVehicle(r);

		if(pos != runs.begin())
		{
			Run& prev_run = *std::prev(pos);
			prev_run.setStopsEndIterator(r.getStops().begin());
		}
		
		auto iter = runs.insert(pos, r);
		
		context.structural_changes_observer.afterRunCreated(iter);
		
		return iter;
	}

	Schedule::~Schedule() 
	{
		clear();
	}

    bool Schedule::isValid() const
    {
		Validator validator;
		validate(validator);
		return validator.getValidationResult() == Validator::ValidationResult::VALID;
    }

	void Schedule::validate(ViolationsConsumer& violations_consumer) const
	{
		schedule_validation_model.getScheduleValidationAlgorithm().validate(*this, violations_consumer);
	}
    
	void Schedule::setActualizationModel(const ScheduleActualizationModel& model)
	{
		this->schedule_actualization_model = model;
	}

	void Schedule::setValidationModel(const ScheduleValidationModel& model)
	{
		this->schedule_validation_model = model;
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
		for (Run& r : runs)
		{
			context.runs_factory.destroyObject(&r);
		}

		runs.clear();
		
		arrival_time_actualizer.setDirty(true);
	}

	const Scene& Schedule::getScene() const
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

	void Schedule::setRunVehicleBinder(Optional<const RunVehicleBinder&> run_vehicle_binder) 
	{
		this->run_vehicle_binder = run_vehicle_binder;
		
		for(Run& r : runs)
		{
			if(run_vehicle_binder) run_vehicle_binder->bindVehicle(r);
			else r.setVehicle(None);
		}
		
		arrival_time_actualizer.setDirty(true);
	}
	
	Optional<const RunVehicleBinder&> Schedule::getRunVehicleBinder() const
	{
		return run_vehicle_binder;
	}
	
    const ScheduleValidationModel& Schedule::getValidationModel() const
    {
		return schedule_validation_model;
	}
	
    const ScheduleActualizationModel& Schedule::getActualizationModel() const
    {
		return schedule_actualization_model;
	}
    
	const Schedule::StopsList& Schedule::getStops() const
	{
		return stops;
	}
	
	Schedule::StopsList& Schedule::getStops()
	{
		return stops;
	}

	bool Schedule::empty() const
	{
		return runs.empty();
	}

	std::pair<Schedule::RunIterator, Schedule::RunDestructor> Schedule::detachRun(ConstRunIterator pos)
	{
		Run& r = *pos;
		
		assert(!r.isDetached());
		
		context.structural_changes_observer.beforeRunDestroyed(pos);
		
		if(pos != runs.begin()) 
		{
			Run& prev_run = *std::prev(pos);
			prev_run.setStopsEndIterator(pos->get().getStops().end());
		}
		
		auto iter = runs.erase(pos);
		
		r.detach();
		
		arrival_time_actualizer.setDirty(true);
		
		return std::make_pair(iter, [&](Run* r){context.runs_factory.destroyObject(r);});
	}
	
	Schedule::RunIterator Schedule::attachRun(ConstRunIterator pos, Run& run)
	{
		assert(run.isDetached());
		assert(run.getSchedule() == *this);
		
		if(pos != runs.begin()) 
		{
			Run& prev_run = *std::prev(pos);
			prev_run.setStopsEndIterator(run.getStops().begin());
		}
		
		run.attach(pos == runs.end() ? stops.end() : pos->get().getStops().begin());
		
		auto iter = runs.insert(pos, run);
		
		arrival_time_actualizer.setDirty(true);
		context.structural_changes_observer.afterRunCreated(iter);
		
		return iter;
	}
	
	Schedule::RunIterator Schedule::destroyRun(ConstRunIterator pos) {
		Run& r = *pos;
		auto result = detachRun(pos);
		result.second(&r);
		return result.first;
	}
	
	bool Schedule::operator==(const Schedule& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Schedule::operator!=(const Schedule& rhs) const
	{
		return !(*this == rhs);
	}

	Schedule::ConstRunIterator Schedule::findRun(const Run& r) const
	{
		return std::find(runs.begin(), runs.end(), r);
	}

	Schedule::RunIterator Schedule::findRun(Run& r)
	{
		return std::find(runs.begin(), runs.end(), r);
	}

	
}
