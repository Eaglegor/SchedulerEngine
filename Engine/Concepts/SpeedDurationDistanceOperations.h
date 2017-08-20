#pragma once

#include "Distance.h"
#include "Duration.h"
#include "Speed.h"
#include <cmath>

#include <Concepts_export.h>

namespace Scheduler
{
	/** 
	 * @brief Calculates the route distance value based on speed and duration
	 * @param speed Speed of vehicle
	 * @param duration Duration of travelling
	 * @return Distance the vehicle will travel over the specified duration having the specified speed
	 */
	CONCEPTS_EXPORT
	Distance operator*(const Speed& speed, const Duration& duration);

	/** 
	 * @brief Calculates the route distance value based on speed and duration
	 * @param speed Speed of the vehicle
	 * @param duration Duration of travelling
	 * @return Distance the vehicle will travel over the specified duration having the specified speed
	 */
	CONCEPTS_EXPORT
	Distance operator*(const Duration& duration, const Speed& speed);

	/** 
	 * @brief Calculates the route duration value based on distance and speed
	 * @param distance Travelled distance
	 * @param speed Speed of the vehicle
	 * @return Duration over which the vehicle will travel the specified distance having the specified speed
	 */
	CONCEPTS_EXPORT
	Duration operator/(const Distance& distance, const Speed& speed);

	/** 
	 * @brief Calculates the vehicle speed based on distance and travel time
	 * @param distance Travelled distance
	 * @param duration Travel time
	 * @return Speed the vehicle should have to travel specified distance over the specified duration
	 */
	CONCEPTS_EXPORT
	Speed operator/(const Distance& distance, const Duration& duration);
}