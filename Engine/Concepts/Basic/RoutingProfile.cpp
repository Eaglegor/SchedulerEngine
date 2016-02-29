#include "RoutingProfile.h"
#include <Utils/Units/SpeedUnits.h>

namespace Scheduler
{

	RoutingProfile::RoutingProfile() : average_speed( Units::speedPerHour(60.0f) ) { }

	RoutingProfile::RoutingProfile(const RoutingProfile &rhs) : average_speed(rhs.average_speed) { }

	const Speed& RoutingProfile::getAverageSpeed() const {
		return average_speed;
	}

	void RoutingProfile::setAverageSpeed(const Speed &average_speed) {
		this->average_speed = average_speed;
	}

	bool RoutingProfile::operator==(const RoutingProfile &rhs) const {
		return average_speed == rhs.average_speed;
	}

	bool RoutingProfile::operator!=(const RoutingProfile &rhs) const {
		return !(*this == rhs);
	}

	RoutingProfile& RoutingProfile::operator=(const RoutingProfile &rhs) {
		this->average_speed = rhs.average_speed;
		return *this;
	}
}