#pragma once

#include <Engine/Routing/RoutingService.h>



namespace Scheduler
{
	/** 
	 * @brief Routing service providing the routes based on "as the crow flies" distance metric
	 * 
	 * @details {"As the crow flies or in a beeline is an idiom for the shortest path between two points 
	 * (on a map, disregarding the vagaries of intervening terrain); the geodesic distance." [Wikipedia]}
	 */
	class CrowFlyRoutingService : public RoutingService
	{
	public:

		/** 
		 * @brief Calculates route information between from and to with respect to the provided routing profile
		 * 
		 * @details {Route distance is calculated using crowfly distance metric. Route duration is calculated
		 * as distance / average speed taken from the vehicle routing profile}
		 * 
		 * @param from Start point of route
		 * @param to End point of route
		 * @param routing_profile Routing profile of the vehicle
		 * 
		 * @return Route information
		 */
		virtual Route calculateRoute(const Site& from, const Site& to,
		                             const RoutingProfile& routing_profile) const override;
	};
}