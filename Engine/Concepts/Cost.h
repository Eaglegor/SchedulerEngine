#pragma once

#include <Engine/Math/FloatEpsilon.h>
#include <cmath>



namespace Scheduler
{
	/** 
	 * @ingroup concepts
	 * 
	 * @brief Represents the value type for cost calculations
	 */
	class Cost
	{
	public:
		/** 
		 * @brief Underlying value type
		 */
		using value_type = float;

		/**
		 * @brief Default constructor
		 */
		Cost( );

		/** 
		 * @brief Value constructor
		 * @param Cost value
		 */
		explicit Cost(value_type value);

		/** 
		 * @Copy constructor
		 * @param cost Source cost
		 */
		Cost(const Cost& cost);

		/**	
		 * @brief Gets the raw value of cost
		 * @return The raw value of cost represented as value_type
		 */
		value_type getValue( ) const;

		/**
		 * @brief Sets the value of cost from the raw value
		 * @param value The raw value represented as value_type
		 */
		void setValue(value_type value);

		/** 
		 * @brief Equality operator
		 * @param rhs Another cost
		 * @return true if the value of this is equal to the value of rhs
		 */
		bool operator==(const Cost& rhs) const;

		/** 
		 * @brief Inequality operator
		 * @param rhs Another cost
		 * @return true if the value of this is not equal to the value of rhs
		 */
		bool operator!=(const Cost& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another cost
		 * @return true if the value of this is greater than the value of rhs
		 */
		bool operator>(const Cost& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another cost
		 * @return true if the value of this is less than the value of rhs
		 */
		bool operator<(const Cost& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another cost
		 * @return true if the value of this is greater or equal to the value of rhs
		 */
		bool operator>=(const Cost& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another cost
		 * @return true if the value of this is less or equal to the value of rhs
		 */
		bool operator<=(const Cost& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another cost
		 * @return New cost calculated as a sum of this and rhs
		 */
		Cost operator+(const Cost& rhs) const;

		/** 
		 * @brief Subtraction operator
		 * @param rhs Another cost
		 * @return New cost calculated as a difference of this and rhs
		 */
		Cost operator-(const Cost& rhs) const;

		/** 
		 * @brief Multiplication operator
		 * @param multiplier Multiplier
		 * @return New cost calculated as this multiplied by multiplier
		 */
		Cost operator*(value_type multiplier) const;

		/** 
		 * @brief Division operator
		 * @param divider Divider
		 * @return New cost calculated as this divided by divider
		 */
		Cost operator/(value_type divider) const;

		/** 
		 * @brief Multiplication operator
		 * @details Multiplies current cost value by multiplier
		 * @param multiplier Multiplier
		 * @return Reference to this
		 */
		Cost& operator*=(value_type multiplier);

		/** 
		 * @brief Division operator
		 * @details Divides current cost value by divider
		 * @param divider Divider
		 * @return Reference to this
		 */
		Cost& operator/=(value_type divider);

		/** 
		 * @brief Addition operator
		 * @details Adds rhs value to the current cost value
		 * @param rhs Another cost
		 * @return Reference to this
		 */
		Cost& operator+=(const Cost& rhs);

		/** 
		 * @brief Subtraction operator
		 * @details Subtracts rhs value from the current cost value
		 * @param rhs Another cost
		 * @return Reference to this
		 */
		Cost& operator-=(const Cost& rhs);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another cost
		 * @return Reference to this
		 */
		Cost& operator=(const Cost& rhs);

		/** 
		 * @brief Assignment operator
		 * @param rhs Raw value represented as value_type
		 * @return Reference to this
		 */
		Cost& operator=(const value_type& rhs);

		/** 
		 * @brief Negation operator
		 * @return New cost which value is represented as negated value of this
		 */
		Cost operator-( ) const;

	private:
		value_type value;
	};
}