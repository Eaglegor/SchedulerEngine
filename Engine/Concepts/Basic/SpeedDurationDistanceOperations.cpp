#include "SpeedDurationDistanceOperations.h"

namespace Scheduler
{

	Distance operator*(const Speed &speed, const Duration &duration) {
		return Distance(speed.getValue() * duration.getValue());
	}

	Distance operator*(const Duration &duration, const Speed &speed) {
		return speed * duration;
	}

	Duration operator/(const Distance &distance, const Speed &speed) {
		return Duration( static_cast<long>(floor(distance.getValue() / speed.getValue().getValue() + 0.5)));
	}

	Speed operator/(const Distance &distance, const Duration &duration) {
		return Speed( distance / duration.getValue() );
	}
}