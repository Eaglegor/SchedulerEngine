#pragma once

#include "Duration.h"
#include <ctime>
#include <limits>



namespace Scheduler
{
	/** 
	 * @brief Represents the single moment in time
	 */
	class TimePoint
	{
	public:
		/** 
		 * @brief Underlying data type
		 */
		using value_type = long;

		/** 
		 * @brief Default constructor
		 * @details The result represents the earliest possible time moment
		 */
		TimePoint( );

		/**
		 * @brief Value constructor
		 * @param value The raw time point value represented as value_type
		 */
		explicit TimePoint(value_type value);

		/** 
		 * @brief Copy constructor
		 * @param rhs Source time point
		 */
		TimePoint(const TimePoint& rhs);

		/** 
		 * @brief Gets the raw value of time point
		 * @return Raw value of time point represented as value_type
		 */
		value_type getValue( ) const;

		/** 
		 * @brief Sets the raw value of time point
		 * @param value Raw value of time point represented as value_type
		 */
		void setValue(value_type value);

		/** 
		 * @brief Equality operator
		 * @param rhs Another time point
		 * @return true if this and rhs represent the same time moment
		 */
		bool operator==(const TimePoint& rhs) const;

		/** 
		 * @brief Equality operator
		 * @param rhs Another time point
		 * @return true if this and rhs represent different time moments
		 */
		bool operator!=(const TimePoint& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another time point
		 * @return true if this represents the time moment earlier than rhs
		 */
		bool operator<(const TimePoint& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another time point
		 * @return true if this represents the time moment later than rhs
		 */
		bool operator>(const TimePoint& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another time point
		 * @return true if this represents the same time moment or the time moment earlier than rhs
		 */
		bool operator<=(const TimePoint& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another time point
		 * @return true if this represents the same time moment or the time moment later than rhs
		 */
		bool operator>=(const TimePoint& rhs) const;

		/** 
		 * @brief Shifts the time point into the future by specified duration
		 * @param duration Duration to shift the time point by
		 * @return New time point value representing time moment of this time point shifted into the future by duration
		 */
		TimePoint operator+(const Duration& duration) const;

		/** 
		 * @brief Shifts the time point into the past by specified duration
		 * @param duration Duration to shift the time point by
		 * @return New time point value representing time moment of this time point shifted into the past by duration
		 */
		TimePoint operator-(const Duration& duration) const;

		/** 
		 * @brief Returns the duration representing the length of time period between this and rhs
		 * @param rhs Another time point
		 * @return Duration value representing the length of time period between this and rhs
		 */
		Duration operator-(const TimePoint& rhs) const;

		/** 
		 * @brief Shifts the time point into the future by specified duration
		 * @param duration Duration to shift the time point by
		 * @return Reference to this
		 */
		TimePoint& operator+=(const Duration& duration);

		/** 
		 * @brief Shifts the time point into the past by specified duration
		 * @param duration Duration to shift the time point by
		 * @return Reference to this
		 */
		TimePoint& operator-=(const Duration& duration);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another time point
		 * @return Reference to this
		 */
		TimePoint& operator=(const TimePoint& rhs);

		/** 
		 * @brief Returns the time point representing the latest possible time moment
		 */
		static TimePoint max( );

		/** 
		 * @brief Returns the time point representing the earliest possible time moment
		 */
		static TimePoint min( );

	private:
		value_type value;
	};
}