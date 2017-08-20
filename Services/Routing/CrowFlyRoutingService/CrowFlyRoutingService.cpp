#define _USE_MATH_DEFINES

#include "CrowFlyRoutingService.h"

#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Concepts/Site.h>
#include <Engine/Concepts/SpeedDurationDistanceOperations.h>

#include <cmath>


namespace Scheduler
{

	Route CrowFlyRoutingService::calculateRoute(const Site& from, const Site& to,
	                                            const RoutingProfile& routing_profile) const
	{
		const float EARTH_RADIUS = 6399.0f;

		float from_lat = fromFixedPoint<Coordinate::PRECISION>(from.getLatitude( ).getValue( ));
		float from_lon = fromFixedPoint<Coordinate::PRECISION>(from.getLongitude( ).getValue( ));

		float to_lat = fromFixedPoint<Coordinate::PRECISION>(to.getLatitude( ).getValue( ));
		float to_lon = fromFixedPoint<Coordinate::PRECISION>(to.getLongitude( ).getValue( ));

		Distance distance(
		    acos(
		        cos((from_lat * (M_PI / 180))) *
		            cos((from_lon * (M_PI / 180))) *
		            cos((to_lat * (M_PI / 180))) *
		            cos((to_lon * (M_PI / 180))) +
		        cos((from_lat * (M_PI / 180))) *
		            sin((from_lon * (M_PI / 180))) *
		            cos((to_lat * (M_PI / 180))) *
		            sin((to_lon * (M_PI / 180))) +
		        sin((from_lat * (M_PI / 180))) *
		            sin((to_lat * (M_PI / 180)))) *
		    EARTH_RADIUS);

		Duration duration = distance / routing_profile.getAverageSpeed( );

		return Route(from, to, distance, duration);
	}
}