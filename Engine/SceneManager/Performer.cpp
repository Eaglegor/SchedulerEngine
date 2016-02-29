#include "Performer.h"

namespace Scheduler {
    Performer::Performer(size_t id) : id(id) {

    }

    size_t Performer::getId() const {
        return id;
    }

    const char* Performer::getName() const {
        return name.c_str();
    }

	ImmutableVector<TimeWindow>& Performer::getAvailabilityWindows() {
        return availability_windows;
    }

    void Performer::setName(const char *name) {
        this->name = name;
    }

    void Performer::setAvailabilityWindows(ImmutableVector<TimeWindow> &time_windows) {
        this->availability_windows = time_windows;
    }

	const Cost& Performer::getDurationUnitCost() const {
		return duration_unit_cost;
	}

	const Cost& Performer::getActivationCost() const {
		return activation_cost;
	}

	void Performer::setDurationUnitCost(const Cost &cost) {
		duration_unit_cost = cost;
	}

	void Performer::setActivationCost(const Cost &cost) {
		activation_cost = cost;
	}

	const ImmutableUnorderedSet<const Attribute*> & Performer::getSkills() const{
		return skills;
	}

	void Performer::setSkills(const ImmutableUnorderedSet<const Attribute *> &skills) {
		this->skills = skills;
	}
}

