#pragma once

#include "Distance.h"
#include "Duration.h"
#include "Site.h"



namespace Scheduler
{
	/** 
	 * @brief Represents a route between two geographical locations
	 */
	class Route
	{
	public:
		/** 
		 * @brief Default constructor
		 * @detais Result is a zero-length route between the geographical points (0,0) and (0,0)
		 */
		Route( );

		/** 
		 * @brief Copy constructor
		 * @param rhs Source route
		 */
		Route(const Route& rhs);

		/** 
		 * @brief Values constructor
		 * @param from Start point of the route
		 * @param to End point of the route
		 * @param distance Distance between from and to
		 * @param duration Time needed to travel between from and to
		 */
		Route(const Site& from, const Site& to, const Distance& distance, const Duration& duration);

		/** 
		 * @brief Gets start point of the route
		 * @return Route start point
		 */
		const Site& getFrom( ) const;

		/** 
		 * @brief Gets end point of the route
		 * @return Route end point
		 */
		const Site& getTo( ) const;

		/** 
		 * @brief Sets start point of the route
		 * @param from Route start point
		 */
		void setFrom(const Site& from);

		/** 
		 * @brief Sets end point of the route
		 * @param to Route end point
		 */
		void setTo(const Site& to);

		/** 
		 * @brief Gets distance between start and end point
		 * @return Distance between start and end point
		 */
		const Distance& getDistance( ) const;

		/** 
		 * @brief Sets distance between start and end point
		 * @param distance Distance between start and end point
		 */
		void setDistance(const Distance& distance);

		/** 
		 * @brief Gets time needed to travel between start and end point
		 * @return Time needed to travel between start and end point
		 */
		const Duration& getDuration( ) const;
		
		/** 
		 * @brief Sets time needed to travel between start and end point
		 * @param duration Time needed to travel between start and end point
		 */
		void setDuration(const Duration& duration);

		/** 
		 * @brief Equality operator
		 * @param rhs Another route
		 * @return {true if this and rhs represent the route between the same start and end points and
		 * have equal distance and duration}
		 */
		bool operator==(const Route& rhs) const;

		/** 
		 * @brief Equality operator
		 * @param rhs Another route
		 * @return true if *this != rhs
		 */
		bool operator!=(const Route& rhs) const;

		/** 
		 * @brief Assignment operator
		 * @param rhs Another route
		 * @return Reference to this
		 */
		Route& operator=(const Route& rhs);

	private:
		Site from;
		Site to;

		Distance distance;
		Duration duration;
	};
}