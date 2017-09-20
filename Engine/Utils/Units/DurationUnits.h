#pragma once

#include <Engine/Concepts/Duration.h>

namespace Scheduler
{
	/**
	 * @ingroup units
	 * @{
	 */

	namespace Units
	{
		/** 
		 * @brief Creates the Duration object describing the passed seconds count
		 * 
		 * @param count The count of seconds
		 * 
		 * @return The Duration object describing the passed seconds count
		 */
		static Duration seconds(long count)
		{
			return Duration(count);
		}

		/** 
		 * @brief Creates the Duration object describing the passed minutes count
		 * 
		 * @param count The count of minutes
		 * 
		 * @return The Duration object describing the passed minutes count
		 */
		static Duration minutes(long count)
		{
			return Duration(count * 60);
		}

		/** 
		 * @brief Creates the Duration object describing the passed hours count
		 * 
		 * @param count The count of hours
		 * 
		 * @return The Duration object describing the passed hours count
		 */
		static Duration hours(long count)
		{
			return Duration(count * 3600);
		}

		/** 
		 * @brief Creates the Duration object describing the passed days count
		 * 
		 * @param count The count of days
		 * 
		 * @return The Duration object describing the passed secdaysonds count
		 */
		static Duration days(long count)
		{
			return Duration(count * 86400);
		}

		/** 
		 * @brief Creates the Duration object describing the passed minutes and seconds count
		 * 
		 * @param minutes_count The count of minutes
		 * @param seconds_count The count of seconds
		 * 
		 * @return The Duration object describing the passed minutes and seconds count
		 */
		static Duration minutes_seconds(long minutes_count, long seconds_count)
		{
			return minutes(minutes_count) + seconds(seconds_count);
		}

		/** 
		 * @brief Creates the Duration object describing the passed hours and minutes count
		 * 
		 * @param hours_count The count of hours
		 * @param minutes_count The count of minutes
		 * 
		 * @return The Duration object describing the passed hours and minutes count
		 */
		static Duration hours_minutes(long hours_count, long minutes_count)
		{
			return hours(hours_count) + minutes(minutes_count);
		}

		/** 
		 * @brief Creates the Duration object describing the passed hours, minutes and seconds count
		 * 
		 * @param hours_count The count of hours
		 * @param minutes_count The count of minutes
		 * @param seconds_count The count of seconds
		 * 
		 * @return The Duration object describing the passed hours, minutes and seconds count
		 */
		static Duration hours_minutes_seconds(long hours_count, long minutes_count, long seconds_count)
		{
			return hours(hours_count) + minutes(minutes_count) + seconds(seconds_count);
		}
	}

	/**
	 * @}
	 */
}