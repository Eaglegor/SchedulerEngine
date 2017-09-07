#pragma once

#include "Speed.h"



namespace Scheduler
{
	/** 
	 * @ingroup concepts
	 * 
	 * @brief Represents the rules used by the routing services during the routes calculation
	 */
	class RoutingProfile
	{
	public:
		/** 
		 * @brief Default constructor
		 */
		RoutingProfile( );

		/** 
		 * @brief Copy constructor
		 * @param rhs Source routing profile
		 */
		RoutingProfile(const RoutingProfile& rhs);

		/** 
		 * @brief Gets average speed of the vehicle
		 * @return Average vehicle speed
		 */
		const Speed& getAverageSpeed( ) const;

		/** 
		 * @brief Sets average speed of the vehicle
		 * @param average_speed New average vehicle speed
		 */
		void setAverageSpeed(const Speed& average_speed);

		/**
		 * @brief Equality operator
		 * @param rhs Another routing profile
		 * @return true if this and rhs have the same average speed
		 */
		bool operator==(const RoutingProfile& rhs) const;

		/**
		 * @brief Inequality operator
		 * @param rhs Another routing profile
		 * @return true if !(*this == rhs)
		 */
		bool operator!=(const RoutingProfile& rhs) const;

		/** 
		 * @brief Assignment operator
		 * @param rhs Another routing profile
		 * @return Reference to this
		 */
		RoutingProfile& operator=(const RoutingProfile& rhs);

	private:
		Speed average_speed;
	};
}