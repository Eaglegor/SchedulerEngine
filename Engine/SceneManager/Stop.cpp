#include "Stop.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include <assert.h>
#include "ScheduleActualizer.h"

namespace Scheduler
{

	Stop::Stop(const Location& location, Run* run):
	location(location),
	run(run),
	schedule_actualizer(nullptr),
	nextStop(nullptr),
	prevStop(nullptr),
	has_actual_route(false)
	{
	}

	const TimeWindow &Stop::getAllocationTime() const {
		schedule_actualizer->actualize();

		return allocation_time;
	}

	void Stop::setAllocationTime(const TimeWindow &time) {
		this->allocation_time = time;
	}

	const Duration &Stop::getDuration() const {
		schedule_actualizer->actualize();

		return duration;
	}

	void Stop::setDuration(const Duration &duration) {
		this->duration = duration;
	}

	void Stop::setNextRoute(const Route &route) {
		this->next_route = route;

		schedule_actualizer->onStopNextRouteChanged(this);
	}

	const Run *Stop::getRun() const {
		return run;
	}

	Run *Stop::getRun() {
		return run;
	}

	const Location &Stop::getLocation() const {
		return location;
	}

	Stop* Stop::getNextStop() const
	{
		return nextStop;
	}

	Stop* Stop::getPrevStop() const
	{
		return prevStop;
	}

	const Route &Stop::getNextRoute() const
	{
		schedule_actualizer->actualize();

		return next_route;
	}

	void Stop::setScheduleActualizer(ScheduleActualizer *actualizer) {
		assert(actualizer);

		this->schedule_actualizer = actualizer;
	}

	void Stop::invalidateRoute()
	{
		has_actual_route = false;
	}

	bool Stop::hasActualRoute() const
	{
		return has_actual_route;
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
		this->setAllocationTime(TimeWindow(time, time + duration));
	}

	void Stop::setEndTime(const TimePoint &time)
	{
		this->setAllocationTime(TimeWindow(time - duration, time));
	}
}