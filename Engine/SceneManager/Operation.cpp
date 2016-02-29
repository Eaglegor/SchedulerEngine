#include "Operation.h"

namespace Scheduler
{
    Operation::Operation(size_t id) :
            id(id),
            order(nullptr)
    {
        
    }

    const char* Operation::getName() const {
        return name.c_str();
    }

    size_t Operation::getId() const {
        return id;
    }

    ImmutableVector<TimeWindow>& Operation::getTimeWindows() const {
        return time_windows;
    }

    const Location& Operation::getLocation() const {
        return location;
    }

    const Duration& Operation::getDuration() const {
        return duration;
    }

    const Order* Operation::getOrder() const {
        return order;
    }

    void Operation::setName(const char *name) {
        this->name = name;
    }

    void Operation::setTimeWindows(ImmutableVector<TimeWindow> &time_windows) {
        this->time_windows = time_windows;
    }

    void Operation::setLocation(const Location &location) {
        this->location = location;
    }

    void Operation::setDuration(const Duration &duration) {
        this->duration = duration;
    }

    void Operation::setOrder(const Order *order) {
        this->order = order;
    }

	void Operation::setLoad(const Capacity &load) {
		this->load = load;
	}

	const Capacity &Operation::getLoad() const {
		return load;
	}
}
