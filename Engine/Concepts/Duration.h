#pragma once

#include <ctime>
#include <limits>

#include <Concepts_export.h>

namespace Scheduler
{
	/** 
	 * @brief Represents duration value
	 */
	class CONCEPTS_EXPORT Duration
	{
	public:
		/** 
		 * @brief Underlying data type
		 */
		using value_type = long;

		/** 
		 * @brief Default constructor
		 */
		Duration( );

		/** 
		 * @brief Value constructor
		 * @param value Raw duration value represented as value_type
		 */
		explicit Duration(value_type value);

		/**
		 * @brief Copy constructor
		 * @param rhs Source duration
		 */
		Duration(const Duration& rhs);

		/** 
		 * @brief Gets the raw duration value
		 * @return The raw value of duration represented as value_type
		 */
		value_type getValue( ) const;

		/** 
		 * @brief Sets the value of duration from the raw value
		 * @param value The raw duration value represented as value_type
		 */
		void setValue(value_type value);

		/** 
		 * @brief Equality operator
		 * @param rhs Another duration
		 * @return true if this value is equal to rhs value
		 */
		bool operator==(const Duration& rhs) const;

		/** 
		 * @brief Equality operator
		 * @param rhs Another duration
		 * @return true if this value is not equal to rhs value
		 */
		bool operator!=(const Duration& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another duration
		 * @return true if this value is less than rhs value
		 */
		bool operator<(const Duration& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another duration
		 * @return true if this value is greater than rhs value
		 */
		bool operator>(const Duration& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another duration
		 * @return true if this value is less than or equal to rhs value
		 */
		bool operator<=(const Duration& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another duration
		 * @return true if this value is greater than or equal to rhs value
		 */
		bool operator>=(const Duration& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another duration
		 * @return New duration calculated as the sum of this and rhs
		 */
		Duration operator+(const Duration& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another duration
		 * @return New duration calculated as the difference of this and rhs
		 */
		Duration operator-(const Duration& rhs) const;

		/** 
		 * @brief Multiplication operator
		 * @param multiplier Multiplier
		 * @return New duration calculated as this multiplied by multiplier
		 */
		Duration operator*(value_type multiplier) const;

		/** 
		 * @brief Division operator
		 * @param divider Divider
		 * @return New duration calculated as this divided by divider
		 */
		Duration operator/(value_type divider) const;

		/** 
		 * @brief Addition operator
		 * @details Adds rhs value to the current value
		 * @param rhs Another duration
		 * @return Reference to this
		 */
		Duration& operator+=(const Duration& rhs);

		/** 
		 * @brief Subtraction operator
		 * @details Subtracts rhs value from the current value
		 * @param rhs Another duration
		 * @return Reference to this
		 */
		Duration& operator-=(const Duration& rhs);

		/** 
		 * @brief Multiplication operator
		 * @details Multiplies current value by rhs value
		 * @param multiplier Multiplier
		 * @return Reference to this
		 */
		Duration& operator*=(value_type multiplier);

		/** 
		 * @brief Division operator
		 * @details Divides current value by rhs value
		 * @param divider Divider
		 * @return Reference to this
		 */
		Duration& operator/=(value_type divider);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another duration
		 * @return Reference to this
		 */
		Duration& operator=(const Duration& rhs);

		/** 
		 * @brief Negation operator
		 * @return New duration with value equal to -this->getValue();
		 */
		Duration operator-( ) const;

		/** 
		 * @brief Returns maximal possible duration value
		 * @return New duration value initialized with maximal possible raw value
		 */
		static Duration max( );

	private:
		value_type value;
	};
}