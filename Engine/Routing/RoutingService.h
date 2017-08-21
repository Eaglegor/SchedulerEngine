#pragma once

#include <Engine/Concepts/Route.h>

namespace Scheduler
{
	class Site;
	class RoutingProfile;

	/**	
	 * @brief Represents the abstract routing service interface
	 * 
	 * @details {Routing service takes 2 geolocations (route start and end points) and the routing profile of vehicle
	 * and returns the route information (distance and driving time). It abstracts the distance matrix concept used
	 * in the logistics optimization problems.}
	 */
	class RoutingService
	{
	public:
		virtual ~RoutingService( )
		{
		}

		/** 
		 * @brief Calculates route information
		 * 
		 * @param from Start point of the resulting route
		 * @param to End point of the resulting route
		 * @routing_profile Vehicle routing profile to calculate the route for
		 * 
		 * @return Route information (driving time and distance)
		 */
		virtual Route calculateRoute(const Site& from, const Site& to, const RoutingProfile& routing_profile) const = 0;
	};
}