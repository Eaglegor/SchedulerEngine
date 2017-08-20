#pragma once

#include <Engine/Math/FloatEpsilon.h>
#include <algorithm>
#include <cmath>

#include <Concepts_export.h>

namespace Scheduler
{
	/**
	 * @brief Represents capacity of vehicle and capacity demands of order
	 */
	class CONCEPTS_EXPORT Capacity
	{
	public:
		/** 
		 * @brief Default constructor
		 */
		Capacity( );
		
		/**
		 * @brief Values constructor
		 * @param d1 First dimension value
		 * @param d2 Second dimension value
		 * @param d3 Third dimension value
		 * @param d4 Fourth dimension value
		 */
		Capacity(float d1, float d2 = 0, float d3 = 0, float d4 = 0);
		
		/**
		 * @brief Copy constructor
		 * @param rhs Source capacity
		 */
		Capacity(const Capacity& rhs);

		/**
		 * @brief Accesses n-th dimension of capacity
		 * @param index Dimension index to get the value
		 * @return Value of capacity dimension
		 */
		const float& operator[](size_t index) const;
		
		/**
		 * @brief Accesses n-th dimension of capacity
		 * @param index Dimension index to get the value
		 * @return Value of capacity dimension
		 */
		float& operator[](size_t index);

		/**
		 * @brief Accesses n-th dimension of capacity
		 * @param dimension_index Dimension index to get the value
		 * @return Value of capacty dimension
		 */
		float getValue(size_t dimension_index) const;
		
		/**
		 * @brief Updates n-th dimension of capacity
		 * @param dimension_index Dimension index to set the value
		 * @param value New value
		 */
		void setValue(size_t dimension_index, float value);
		
		/**
		 * @brief Updates all dimensions of capacity
		 * @param d1 First dimension value
		 * @param d2 Second dimension value
		 * @param d3 Third dimension value
		 * @param d4 Fourth dimension value
		 */
		void setValues(float d1, float d2 = 0, float d3 = 0, float d4 = 0);

		/**
		 * @brief Equality operator
		 * @param rhs Another capacity
		 * @return true if all values of this are equal to all values of rhs (respecting float EPSILON value)
		 */
		bool operator==(const Capacity& rhs) const;
		
		/**
		 * @brief Inequality operator
		 * @param rhs Another capacity
		 * @return true if !(*this == rhs)
		 */
		bool operator!=(const Capacity& rhs) const;

		/**
		 * @brief Comparison operator
		 * @param rhs Another capacity
		 * @return true if all values of this are less than all values of rhs (respecting float EPSILON value)
		 */
		bool operator<(const Capacity& rhs) const;
		
		/**
		 * @brief Comparison operator
		 * @param rhs Another capacity
		 * @return true if all values of this are greater than all values of rhs (respecting float EPSILON value)
		 */
		bool operator>(const Capacity& rhs) const;
	
		/**
		 * @brief Comparison operator
		 * @param rhs Another capacity
		 * @return true if (*this == rhs) or (*this < rhs)
		 */
		bool operator<=(const Capacity& rhs) const;
		
		/**
		 * @brief Comparison operator
		 * @param rhs Another capacity
		 * @return true if (*this == rhs) or (*this > rhs)
		 */
		bool operator>=(const Capacity& rhs) const;

		/**
		 * @brief Addition operator
		 * @param rhs Another capacity
		 * @return New capacity where all values are calculated as (retval[i] = *this[i] - rhs[i])
		 */
		Capacity operator+(const Capacity& rhs) const;
		
		/**
		 * @brief Substraction operator
		 * @param rhs Another capacity
		 * @return New capacity where all values are calculated as (retval[i] = *this[i] - rhs[i])
		 */
		Capacity operator-(const Capacity& rhs) const;

		/**
		 * @brief Adds the values of mathing dimensions of rhs to the values of this
		 * @details Updates the values of this so that (*this[i] = *this[i] + rhs[i])
		 * @param rhs Another capacity
		 * @return Reference to this
		 */
		Capacity& operator+=(const Capacity& rhs);
		
		/**
		 * @brief Subtracts the values of mathing dimensions of rhs from the values of this
		 * * @details Updates the values of this so that (*this[i] = *this[i] - rhs[i])
		 * @param rhs Another capacity
		 * @return Reference to this
		 */
		Capacity& operator-=(const Capacity& rhs);

		/**
		 * @brief Sets all values of this using matching dimensions of passed capacity
		 * @details Updates the values of this so that (*this[i] = rhs[i])
		 * @param rhs Another capacity
		 * @return Reference to this
		 */
		Capacity& operator=(const Capacity& rhs);

		/**
		 * @brief Negation operator
		 * @return New capacity where all values are calculated as (retval[i] = -*this[i])
		 */
		Capacity operator-( ) const;

	private:
		float capacity[4];
	};
}
