#pragma once

#include <Engine/Routing/RoutingService.h>
#include <functional>
#include <unordered_map>

#include <TspLibRoutingService_export.h>

namespace Scheduler
{
	/** 
	 * @brief Routing service providing routes from the custom distance matrix
	 * 
	 * @details {This routing service is intended to be used in the TSPLIB benchmarks encoded in XML format 
	 * (https://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/XML-TSPLIB/instances).
	 * There are no real coordinates of the orders but only a distance matrix provided so for the 
	 * from and end points only latitude coordinate is used and it's encoding the order index (not real latitude).
	 * 
	 * This distance matrix is filled by the TspLibSceneLoader}
	 */
	class TSPLIBROUTINGSERVICE_EXPORT TspLibRoutingService : public RoutingService
	{
	public:
		/** 
		 * @brief Default constructor
		 */
		TspLibRoutingService( );
		~TspLibRoutingService( );

		/** 
		 * @brief Initializes the distance matrix memory storage
		 * 
		 * @param nodes_count Rows/Columns count in the matrix (count of orders in the problem instance)
		 */
		void init(std::size_t nodes_count);

		/** 
		 * @brief Returns a route information from the stored distance matrix
		 * 
		 * @details {Route distance is taken from the distance matrix. Route duration is calculated
		 * as distance / average speed taken from the vehicle routing profile}
		 * 
		 * @param from Start point of the route (latitude is used as index, longitude is ignored)
		 * @param from End point of the route (latitude is used as index, longitude is ignored)
		 * @param routing_profile Vehicle routing profile
		 * 
		 * @return Route information
		 */
		virtual Route calculateRoute(const Site& from, const Site& to, const RoutingProfile& routing_profile) const override;

		/** 
		 * @brief Inserts the new distance value into the distance matrix
		 * 
		 * @param ia Index of route start point
		 * @param ib Index of route end point
		 * @param distance Distance to store in the matrix
		 */
		void insertRoute(std::size_t ia, std::size_t ib, float distance);

	private:
		float* routes;
		std::size_t nodes_count;
	};
}