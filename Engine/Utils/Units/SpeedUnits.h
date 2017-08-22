#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/Concepts/Speed.h>

namespace Scheduler
{
	namespace Units
	{
		/** 
		 * @brief Creates the Speed object using the travelled distance per hour
		 * 
		 * @param distance The distance travelled per hour
		 * 
		 * @return The Speed object matching the passed distance per hour
		 */
		static Speed speedPerHour(const Distance& distance)
		{
			return Speed(distance / 3600.0f);
		}

		/** 
		 * @brief Creates the Speed object using the travelled distance per hour
		 * 
		 * @param distance The distance travelled per hour
		 * 
		 * @return The Speed object matching the passed distance per hour
		 */
		static Speed speedPerHour(float distance)
		{
			return Speed(Distance(distance) / 3600.0f);
		}
	}
}