#pragma once

#include <Engine/Math/FixedPointMath.h>
#include <cstddef>

#include <Concepts_export.h>

namespace Scheduler
{
	/**
	 * @brief Represents single geographic coordinate (latitude or longitude) as a fixed point number
	 */
	class CONCEPTS_EXPORT Coordinate
	{
	public:
		/**
		 * @brief Precision of fixed point math
		 */
		static const size_t PRECISION = 6;

		/** 
		 * @brief Default constructor
		 */
		Coordinate( );

		/** 
		 * @brief Value constructor
		 * @param value Coordinate value expressed as fixed-point number
		 */
		explicit Coordinate(FixedPointType value);

		/** 
		 * @brief Copy constructor
		 * @param rhs Source coordinate
		 */
		Coordinate(const Coordinate& rhs);

		/** 
		 * @brief Gets a value of coordinate
		 * @return Fixed-point value representing the coordinate
		 */
		FixedPointType getValue( ) const;

		/** 
		 * @brief Sets a velue of coordinate
		 * @param value New coordinate value expressed as fixed-point number
		 */
		void setValue(FixedPointType value);

		/**
		 * @brief Equality operator
		 * @param rhs Another coordinate
		 * @return true if the values of this and rhs are equal
		 */
		bool operator==(const Coordinate& rhs) const;

		/**
		 * @brief Inequality operator
		 * @param rhs Another coordinate
		 * @return true if !(*this == rhs)
		 */
		bool operator!=(const Coordinate& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another coordinate
		 * @return true if the value of *this is less than the value of rhs
		 */
		bool operator<(const Coordinate& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another coordinate
		 * @return true if the value of *this is greater than the value of rhs
		 */
		bool operator>(const Coordinate& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another coordinate
		 * @return true if (*this < rhs) or (*this == rhs)
		 */
		bool operator<=(const Coordinate& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another coordinate
		 * @return true if (*this > rhs) or (*this == rhs)
		 */
		bool operator>=(const Coordinate& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another coordinate
		 * @return New coordinate calculated as the sum of *this and rhs
		 */
		Coordinate operator+(const Coordinate& rhs) const;

		/** 
		 * @brief Subtraction operator
		 * @param rhs Another coordinate
		 * @return New coordinate calculated as the difference of *this and rhs
		 */
		Coordinate operator-(const Coordinate& rhs) const;

		/** 
		 * @brief Multiplication operator
		 * @param rhs Another coordinate
		 * @return New coordinate calculated as *this times rhs
		 */
		Coordinate operator*(float multiplier) const;

		/** 
		 * @brief Division operator
		 * @param rhs Another coordinate
		 * @return New coordinate calculated as the *this divided by rhs
		 */
		Coordinate operator/(float divider) const;

		/** 
		 * @brief Updates the value adding rhs value to it
		 * @param rhs Another coordinate
		 * @return Reference to this
		 */
		Coordinate& operator+=(const Coordinate& rhs);

		/**
		 * @brief Updates the value subtraction rhs value from it
		 * @param rhs Another coordinate
		 * @return Reference to this
		 */
		Coordinate& operator-=(const Coordinate& rhs);

		/** 
		 * @brief Updates the value multiplying it by rhs value
		 * @param rhs Another coordinate
		 * @return Reference to this
		 */
		Coordinate& operator*=(float multiplier);

		/** 
		 * @brief Updates the value dividing it by value
		 * @param rhs Another coordinate
		 * @return Reference to this
		 */
		Coordinate& operator/=(float divider);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another coordinate
		 * @return Reference to this
		 */
		Coordinate& operator=(const Coordinate& rhs);

		/** 
		 * @brief Negation operator
		 * @param rhs Another coordinate
		 * @return New coordinate with value equal to -(*this.getValue())
		 */
		Coordinate operator-( ) const;

		/**
		 * @brief Creates the new coordinate from the floating point value
		 * @details Converts the passed floating point value to the fixed 
		 * point representation and then creates new coordinate from it
		 * @return New coordinate initialized by passed value
		 */
		static Coordinate createFromFloat(float value);

	private:
		FixedPointType value;
	};
}