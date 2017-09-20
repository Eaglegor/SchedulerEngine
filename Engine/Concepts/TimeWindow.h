#pragma once

#include "TimePoint.h"
#include <limits>



namespace Scheduler
{
	/** 
	 * @ingroup concepts
	 * 
	 * @brief Represents the time window
	 * 
	 * @details Time window is a period of time between 2 specified time points
	 */
	class TimeWindow
	{
	public:
		/** 
		 * @brief Default constructor
		 * 
		 * @details Represents the zero-length time window starting at time point 0 and ending at time point 0
		 */
		TimeWindow( );

		/** 
		 * @brief Copy constructor
		 * 
		 * @param rhs Source time window
		 */
		TimeWindow(const TimeWindow& rhs);

		/** 
		 * @brief Values constructor
		 * 
		 * @param start_time Starting time of the time window
		 * @param end_time Ending time of the time window
		 */
		TimeWindow(const TimePoint& start_time, const TimePoint& end_time);

		/**
		 * @brief Gets the start time of the time window
		 * 
		 * @return Start time of time window
		 */
		const TimePoint& getStartTime( ) const;

		/**
		 * @brief Gets the end time of the time window
		 * 
		 * @return End time of time window
		 */
		const TimePoint& getEndTime( ) const;

		/**
		 * @brief Sets the start time of the time window
		 * 
		 * @param start_time Start time of time window
		 */
		void setStartTime(const TimePoint& start_time);

		/**
		 * @brief Sets the end time of the time window
		 * 
		 * @param end_time End time of time window
		 */
		void setEndTime(const TimePoint& end_time);

		/** 
		 * @brief Gets the duration of time window (the length of time period between the start and end time points)
		 * 
		 * @return Duration of the time window
		 */
		Duration getDuration( ) const;

		/** 
		 * @brief Equality operator
		 * 
		 * @param rhs Another time window
		 * 
		 * @return true if this and rhs represent the same time period
		 */
		bool operator==(const TimeWindow& rhs) const;

		/** 
		 * @brief Inequality operator
		 * 
		 * @param rhs Another time window
		 * 
		 * @return true if this and rhs represent different time periods
		 */
		bool operator!=(const TimeWindow& rhs) const;

		/** 
		 * @brief Assignment operator
		 * 
		 * @param rhs Another time window
		 * 
		 * @return Reference to this
		 */
		TimeWindow& operator=(const TimeWindow& rhs);

	private:
		TimePoint start_time;
		TimePoint end_time;
	};
}