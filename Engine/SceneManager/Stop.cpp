#include "Stop.h"
#include "Run.h"
#include "Location.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include "ScheduleActualizationModel.h"
#include "RouteActualizer.h"
#include "ArrivalTimeActualizer.h"
#include "DurationActualizer.h"
#include "ScheduleValidationModel.h"
#include "Extensions/StopValidationAlgorithm.h"
#include <assert.h>

namespace Scheduler
{

	Stop::Stop(Run* run):
	run(run),
	nextStop(nullptr),
	prevStop(nullptr),
	schedule_actualizaton_model(nullptr),
	schedule_validation_model(nullptr),
	arrival_time_actualizer(nullptr)
	{
	}

	const TimeWindow &Stop::getAllocationTime() const {
		return allocation_time.get();
	}

	void Stop::setAllocationTime(const TimeWindow &time) {
		this->allocation_time = time;
	}

	const Duration &Stop::getDuration() const {
		return duration.get();
	}

	void Stop::setDuration(const Duration &duration) {
		this->duration = duration;
	}

	void Stop::setNextRoute(const Route &route) {
		this->next_route = route;
	}

	const Run *Stop::getRun() const {
		return run;
	}

	Run *Stop::getRun() {
		return run;
	}

	Stop* Stop::next() const
	{
		return nextStop;
	}

	Stop* Stop::prev() const
	{
		return prevStop;
	}

	void Stop::setScheduleActualizationModel(Scheduler::ScheduleActualizationModel* model, Scheduler::ArrivalTimeActualizer* arrival_time_actualizer, Scheduler::DurationActualizer* duration_actualizer)
	{
		this->schedule_actualizaton_model = model;
		
		route_actualizer = model ? RouteActualizer(model->getRouteActualizationAlgorithm(), this) : RouteActualizer();
		this->next_route.setActualizer(&route_actualizer);
		
		this->arrival_time_actualizer = arrival_time_actualizer;
		this->allocation_time.setActualizer(arrival_time_actualizer);
		
		this->duration_actualizer = duration_actualizer;
		this->duration.setActualizer(duration_actualizer);
	}

	void Stop::setScheduleValidationModel(ScheduleValidationModel* model)
	{
		this->schedule_validation_model = model;
	}

	const Route &Stop::getNextRoute() const
	{
		return next_route.get();
	}

	void Stop::setNext(Stop* stop)
	{
		this->nextStop = stop;
		if(stop == nullptr || stop->getLocation() != getLocation()) route_actualizer.setDirty(true);
	}

	void Stop::setPrev(Stop* stop)
	{
		this->prevStop = stop;
	}

	bool Stop::isValid() const
	{
		if (schedule_validation_model == nullptr || schedule_validation_model->getStopValidationAlgorithm() == nullptr) return true;
		return schedule_validation_model->getStopValidationAlgorithm()->isValid(this);
	}

	void Stop::setStartTime(const TimePoint &time)
	{
		this->setAllocationTime(TimeWindow(time, time + duration.get()));
	}

	void Stop::setEndTime(const TimePoint &time)
	{
		this->setAllocationTime(TimeWindow(time - duration.get(), time));
	}
}
