#define _USE_MATH_DEFINES

#include "EuclideanRoutingService.h"

#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/SpeedDurationDistanceOperations.h>
#include <Engine/Concepts/Site.h>

#include <cmath>


namespace Scheduler
{

	Route EuclideanRoutingService::calculateRoute(const Site &from, const Site &to,
														const RoutingProfile &routing_profile) {
		
		auto x1 = fromFixedPoint<Coordinate::PRECISION>(from.getLatitude().getValue());
		auto y1 = fromFixedPoint<Coordinate::PRECISION>(from.getLongitude().getValue());
		auto x2 = fromFixedPoint<Coordinate::PRECISION>(to.getLatitude().getValue());
		auto y2 = fromFixedPoint<Coordinate::PRECISION>(to.getLongitude().getValue());
		
		auto dx = x2 - x1;
		auto dy = y2 - y1;
		
        Distance distance( static_cast<float>(sqrt(dx*dx + dy*dy)) );
		Duration duration = distance / routing_profile.getAverageSpeed();

		return Route(from, to, distance, duration);
	}
}
