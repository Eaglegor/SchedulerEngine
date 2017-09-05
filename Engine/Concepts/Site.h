#pragma once

#include "Coordinate.h"
#include <cmath>



namespace Scheduler
{
	/** 
	 * @brief Represents a pair of geographic coordinates (latitude and longitude)
	 */
	class Site
	{
	public:
		/** 
		 * @brief Default constructor
		 * @details Constructs the site representing the geographic location (0,0)
		 */
		Site( );

		/** 
		 * @brief Copy constructor
		 * @param rhs Source site
		 */
		Site(const Site& rhs);

		/** 
		 * @brief Values constructor
		 * @param latitude Latitude of geographic location
		 * @param longitude Longitude of geographic location
		 */
		Site(Coordinate latitude, Coordinate longitude);

		/** 
		 * @brief Gets the latitude
		 * @return Coordinate representing latitude value
		 */
		Coordinate getLatitude( ) const;

		/** 
		 * @brief Gets the latitude
		 * @return Coordinate representing the longitude value
		 */
		Coordinate getLongitude( ) const;

		/** 
		 * @brief Sets the latitude
		 * @param latitude Latitude value
		 */
		void setLatitude(const Coordinate& latitude);

		/** 
		 * @brief Sets the longitude
		 * @param longitude Longitude value
		 */
		void setLongitude(const Coordinate& longitude);

		/** 
		 * @brief Equality operator
		 * @param rhs Another site
		 * @return true if both latitude and longitude of this and rhs are equal
		 */
		bool operator==(const Site& rhs) const;

		/** 
		 * @brief Inequality operator
		 * @param rhs Another site
		 * @return true if !(*this == rhs)
		 */
		bool operator!=(const Site& rhs) const;

		/** 
		 * @brief Assignment operator
		 * @param rhs Another site
		 * @return Reference to this
		 */
		Site& operator=(const Site& rhs);

	private:
		Coordinate latitude;
		Coordinate longitude;
	};
}