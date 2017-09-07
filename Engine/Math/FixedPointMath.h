#pragma once

#include "IntegerPower.h"
#include <cmath>
#include <cstdint>

namespace Scheduler
{
	/**
	 * @ingroup math
	 * @{
	 */

	/** 
	 * @brief Type capable to store the fixed point values used by the engine
	 */
	using FixedPointType = int32_t;

	/** 
	 * @brief Converts the passed floating point value to the fixed point representation
	 * 
	 * @tparam PRECISION Precision to store the resulting type
	 * @param value Floating point value to convert
	 * 
	 * @return Fixed point representation of value
	 */
	template <std::size_t PRECISION>
	FixedPointType toFixedPoint(float value)
	{
		return static_cast<int32_t>(std::trunc(value * IntegerPower<10, PRECISION>::val));
	}

	/** 
	 * @brief Converts the passed fixed point value to the floating point representation
	 * 
	 * @tparam PRECISION Precision of the passed value
	 * @param value Fixed point value to convert
	 * 
	 * @return Floating point representation of value
	 */
	template <std::size_t PRECISION>
	float fromFixedPoint(FixedPointType value)
	{
		return static_cast<float>(value) / static_cast<float>(IntegerPower<10, PRECISION>::val);
	}

	/**
	 * @}
	 */
}