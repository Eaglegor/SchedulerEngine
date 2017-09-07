#pragma once

#include <Engine/Concepts/Distance.h>
#include <Engine/Math/FloatEpsilon.h>



namespace Scheduler
{
	/** 
	 * Represents the vehicle speed (distance travelled over the single time unit)
	 */
	class Speed
	{
	public:
		/** 
		 * @brief Default constructor
		 * @details constructs the zero speed value
		 */
		Speed( );

		/** 
		 * @brief Value constructor
		 * @param value Distance travelled over the single time unit
		 */
		explicit Speed(const Distance& value);

		/** 
		 * @brief Copy constructor
		 * @param rhs Source speed
		 */
		Speed(const Speed& rhs);

		/** 
		 * @brief Gets the speed value
		 * @return Distance travelled over the single time unit
		 */
		const Distance& getValue( ) const;

		/** 
		 * @brief Sets the speed value
		 * @param value Distance travelled over the single time unit
		 */
		void setValue(const Distance& value);

		/** 
		 * @brief Equality operator
		 * @param rhs Another speed
		 * @return true if the values of this and rhs are equal
		 */
		bool operator==(const Speed& rhs) const;

		/** 
		 * @brief Inequality operator
		 * @param rhs Another speed
		 * @return true if !(*this == rhs)
		 */
		bool operator!=(const Speed& rhs) const;


		/** 
		 * @brief Comparison operator
		 * @param rhs Another speed
		 * @return {true if the distance value of this is greater than distance value of rhs 
		 * considering the same time units}
		 */
		bool operator>(const Speed& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another speed
		 * @return {true if the distance value of this is less than distance value of rhs 
		 * considering the same time units}
		 */
		bool operator<(const Speed& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another speed
		 * @return {true if the distance value of this is greater than or equal to the distance value of rhs 
		 * considering the same time units}
		 */
		bool operator>=(const Speed& rhs) const;

		/** 
		 * @brief Comparison operator
		 * @param rhs Another speed
		 * @return {true if the distance value of this is less than or equal to the distance value of rhs 
		 * considering the same time units}
		 */
		bool operator<=(const Speed& rhs) const;

		/** 
		 * @brief Addition operator
		 * @param rhs Another speed
		 * @return New speed value calculated as the sum of this and rhs
		 */
		Speed operator+(const Speed& rhs) const;

		/** 
		 * @brief Subtraction operator
		 * @param rhs Another speed
		 * @return New speed value calculated as the difference between this and rhs
		 */
		Speed operator-(const Speed& rhs) const;

		/** 
		 * @brief Multiplication operator
		 * @param multiplier Multiplier
		 * @return New speed value calculated as this multiplied by multiplier
		 */
		Speed operator*(float multiplier) const;

		/** 
		 * @brief Division operator
		 * @param divider Divider
		 * @return New speed value calculated as this divided by multiplier
		 */
		Speed operator/(float divider) const;

		/** 
		 * @brief Addition operator
		 * @details Adds the value of rhs to the current value
		 * @param rhs Another speed
		 * @return Reference to this
		 */
		Speed& operator+=(const Speed& rhs);

		/** 
		 * @brief Subtraction operator
		 * @details Subtracts the value of rhs from the current value
		 * @param rhs Another speed
		 * @return Reference to this
		 */
		Speed& operator-=(const Speed& rhs);

		/** 
		 * @brief Multiplication operator
		 * @details Multiplies the value of this by multiplier
		 * @param multiplier Multiplier
		 * @return Reference to this
		 */
		Speed& operator*=(float multiplier);

		/** 
		 * @brief Division operator
		 * @details Divides the value of this by divider
		 * @param divider Divider
		 * @return Reference to this
		 */
		Speed& operator/=(float divider);

		/** 
		 * @brief Assignment operator
		 * @param rhs Another speed
		 * @return Reference to this
		 */
		Speed& operator=(const Speed& rhs);

		/** 
		 * @brief Negation operator
		 * @return New speed value calculated as -this->getValue()
		 */
		Speed operator-( ) const;

	private:
		Distance value;
	};
}