#pragma once

#include "Speed.h"

namespace Scheduler {
	class RoutingProfile {
	public:
		RoutingProfile();
		RoutingProfile(const RoutingProfile &rhs);

		const Speed& getAverageSpeed() const;
		void setAverageSpeed(const Speed &average_speed);

		bool operator==(const RoutingProfile &rhs) const;
		bool operator!=(const RoutingProfile &rhs) const;

		RoutingProfile& operator=(const RoutingProfile &rhs);

	private:
		Speed average_speed;
	};
}