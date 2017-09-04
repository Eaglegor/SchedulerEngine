#pragma once

#include <Engine/Routing/RoutingService.h>



namespace Scheduler
{
	/** 
	 * @brief Routing service providing the routes calculated as described in the Taillard CVRP benchmark
	 * 
	 * @details {Euclidean distance is used in Taillard CVRP benchmark datasets}
	 */
	class TaillardRoutingService : public RoutingService
	{
	public:
		/** 
		 * @brief Calculates route information using the euclidean distance metric
		 * 
		 * @details {Route distance is calculated using euclidean distance metric. Route duration is calculated
		 * as distance / average speed taken from the vehicle routing profile}
		 * 
		 * @param from Start point of the route
		 * @param to End point of the route
		 * @param routing_profile Vehicle routing profile
		 * 
		 * @return Route information
		 */
		virtual Route calculateRoute(const Site& from, const Site& to,
		                             const RoutingProfile& routing_profile) const override;
	};
}