#include "Stop.h"
#include <Utils/Collections/Algorithms.h>
#include <assert.h>

namespace Scheduler
{

	Stop::Stop(size_t id, const Location& location, Run* run):
	id(id),
	location(location),
	run(run)
	{
	}

	size_t Stop::getId() const {
		return id;
	}

	const ImmutableUnorderedSet<const Operation *>& Stop::getOperations() const {
		return operations;
	}

	void Stop::addOperation(const Operation *operation) {
		assert(operation->getLocation() == location);
		operations.insert(operation);
	}

	void Stop::removeOperation(const Operation *operation) {
		operations.erase(operation);
	}

	bool Stop::containsOperation(const Operation *operation) const {
		return std::contains_key(operations, operation);
	}

	const TimeWindow &Stop::getAllocationTime() const {
		return allocation_time;
	}

	void Stop::setAllocationTime(const TimeWindow &time) {
		this->allocation_time = time;
	}

	const Duration &Stop::getDuration() const {
		return duration;
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

	const Location &Stop::getLocation() const {
		return location;
	}

	const Route &Stop::getNextRoute()
	{
		return next_route;
	}
}