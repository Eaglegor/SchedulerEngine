#pragma once

#include "IntegerPower.h"
#include <cstdint>
#include <cmath>

namespace Scheduler
{
	using FixedPointType = int32_t;

	template<size_t PRECISION>
	FixedPointType toFixedPoint(float value)
	{
		return static_cast<int32_t>(std::trunc(value * IntegerPower<10, PRECISION>::val));
	}

	template<size_t PRECISION>
	float fromFixedPoint(FixedPointType value)
	{
		return static_cast<float>(value) / static_cast<float>(IntegerPower<10, PRECISION>::val);
	}
}