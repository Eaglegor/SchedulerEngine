#define _USE_MATH_DEFINES

#include "TaillardRoutingService.h"

#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/SpeedDurationDistanceOperations.h>
#include <Engine/Concepts/Site.h>

#include <cmath>


namespace Scheduler
{

	Route TaillardRoutingService::calculateRoute(const Site &from, const Site &to,
														const RoutingProfile &routing_profile) const {
		
		auto x1 = from.getLatitude().getValue();
		auto y1 = from.getLongitude().getValue();
		auto x2 = to.getLatitude().getValue();
		auto y2 = to.getLongitude().getValue();
		
		auto dx = x2 - x1;
		auto dy = y2 - y1;
		
        Distance distance( static_cast<float>(sqrt(dx*dx + dy*dy)) );
		Duration duration = distance / routing_profile.getAverageSpeed();

		return Route(from, to, distance, duration);
	}
}
