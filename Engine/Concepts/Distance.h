#pragma once

#include <Engine/Math/FloatEpsilon.h>
#include <cmath>



namespace Scheduler
{
	/** 
	 * @ingroup concepts
	 * 
	 * @brief Represents the distance value
	 */
	class Distance
	{
	public:
		/**
		 * @brief Underlying data type
		 */
		using value_type = float;

		/** 
		 * @brief Default constructor
		 */
		Distance( );

		/** 
		 * @brief Value constructor
		 * @param value Raw distance value represented as value_type
		 */
		explicit Distance(value_type value);

		/** 
		 * @brief Copy constructor
		 * @param rhs Source distance
		 */
		Distance(const Distance& rhs);

		/** 
		 * @brief Returns the raw value of distance
		 * @return The raw value of distance represented as value_type
		 */
		value_type getValue( ) const;

		/** 
		 * @brief Sets the value of distance from the raw value
		 * @param value The raw value of distance represented as value_type
		 */
		void setValue(float value);

		/** 
		 * @brief Equality operator
		 * @param rhs Another distance
		 * @return true if the values of this and rhs are equal
		 */
		bool operator==(const Distance& rhs) const;

		/** 
		 * @brief Inequality operator
		 * @param rhs Another distance
		 * @return true if the values of this and rhs are not equal
		 */
		bool operator!=(const Distance& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another distance
		 * @return true if the value of this is greater than the value of rhs
		 */
		bool operator>(const Distance& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another distance
		 * @return true if the value of this is less than the value of rhs
		 */
		bool operator<(const Distance& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another distance
		 * @return true if the value of this is greater or equal to the value of rhs
		 */
		bool operator>=(const Distance& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another distance
		 * @return true if the value of this is less or equal than the value of rhs
		 */
		bool operator<=(const Distance& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another distance
		 * @return New distance value calculated as a sum of this and rhs
		 */
		Distance operator+(const Distance& rhs) const;

		/** 
		 * @brief Subtraction operator
		 * @param rhs Another distance
		 * @return New distance value calculated as a differenct of this and rhs
		 */
		Distance operator-(const Distance& rhs) const;

		/** 
		 * @brief Multiplication operator
		 * @param multiplier Multiplier
		 * @return New distance value calculated as this multiplied by multiplier
		 */
		Distance operator*(value_type multiplier) const;

		/** 
		 * @brief Division operator
		 * @param divider Divider
		 * @return New distance value calculated as this divided by divider
		 */
		Distance operator/(value_type divider) const;

		/** 
		 * @brief Addition operator
		 * @details Adds rhs value to the current value
		 * @param rhs Another distance
		 * @return Reference to this
		 */
		Distance& operator+=(const Distance& rhs);

		/** 
		 * @brief Subtraction operator
		 * @details Subtracts rhs value from the current value
		 * @param rhs Another distance
		 * @return Reference to this
		 */
		Distance& operator-=(const Distance& rhs);

		/** 
		 * @brief Multiplication operator
		 * @details Multiplies this value by multiplier
		 * @param multiplier Multiplier
		 * @return Reference to this
		 */
		Distance& operator*=(value_type multiplier);

		/** 
		 * @brief Division operator
		 * @details Divides this value by divider
		 * @param divider Divider
		 * @return Reference to this
		 */
		Distance& operator/=(value_type divider);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another distance
		 * @return Reference to this
		 */
		Distance& operator=(const Distance& rhs);

		/** 
		 * @brief Negation operator
		 * @return New distance with value calculated as -this->value
		 */
		Distance operator-( ) const;

	private:
		value_type value;
	};
}