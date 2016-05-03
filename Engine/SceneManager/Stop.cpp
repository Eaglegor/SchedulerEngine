#include "Stop.h"
#include "Run.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include "ScheduleActualizationModel.h"
#include "RouteActualizer.h"
#include "ArrivalTimeActualizer.h"
#include "DurationActualizer.h"
#include <assert.h>

namespace Scheduler
{

	Stop::Stop(Run* run):
	run(run),
	nextStop(nullptr),
	prevStop(nullptr)
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

	Stop* Stop::getNextStop() const
	{
		return nextStop;
	}

	Stop* Stop::getPrevStop() const
	{
		return prevStop;
	}

	void Stop::setActualizationModel(ScheduleActualizationModel* actualization_model)
	{
		this->next_route.setActualizer(actualization_model ? RouteActualizer(actualization_model->getRouteActualizationAlgorithm(), this) : RouteActualizer());
		this->allocation_time.setActualizer(actualization_model ? ArrivalTimeActualizer(actualization_model->getArrivalTimeActualizationAlgorithm(), this->getRun()->getSchedule()) : ArrivalTimeActualizer());
		this->duration.setActualizer(actualization_model ? DurationActualizer(actualization_model->getDurationActualizationAlgorithm(), this) : DurationActualizer());
	}

	const Route &Stop::getNextRoute() const
	{
		return next_route.get();
	}

	void Stop::invalidateRoute()
	{
        next_route.setActual(false);
	}

	void Stop::invalidateArrivalTime()
	{
		allocation_time.setActual(false);
	}

	void Stop::invalidateDuration()
	{
		duration.setActual(false);
	}

	void Stop::setNextStop(Stop* stop)
	{
		this->nextStop = stop;
	}

	void Stop::setPrevStop(Stop* stop)
	{
		this->prevStop = stop;
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
