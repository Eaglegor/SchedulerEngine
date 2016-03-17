#define _USE_MATH_DEFINES

#include "CrowFlyRoutingService.h"

#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/SpeedDurationDistanceOperations.h>
#include <Engine/Concepts/Location.h>

#include <cmath>


namespace Scheduler
{

	Route CrowFlyRoutingService::calculateRoute(const Location &from, const Location &to,
														const RoutingProfile &routing_profile) {
		const float EARTH_RADIUS = 6399.0f;

		Distance distance(
				acos(
						cos((from.getLatitude() * (M_PI / 180)).getValue()) *
						cos((from.getLongitude() * (M_PI / 180)).getValue()) *
						cos((to.getLatitude() * (M_PI / 180)).getValue()) *
						cos((to.getLongitude() * (M_PI / 180)).getValue())
						+
						cos((from.getLatitude() * (M_PI / 180)).getValue()) *
						sin((from.getLongitude() * (M_PI / 180)).getValue()) *
						cos((to.getLatitude() * (M_PI / 180)).getValue()) *
						sin((to.getLongitude() * (M_PI / 180)).getValue())
						+
						sin((from.getLatitude() * (M_PI / 180)).getValue()) *
						sin((to.getLatitude() * (M_PI / 180)).getValue())
				) * EARTH_RADIUS
		);

		Duration duration = distance / routing_profile.getAverageSpeed();

		return Route(from, to, distance, duration);
	}
}