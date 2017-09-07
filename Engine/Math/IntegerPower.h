#pragma once

#include <cstdint>

namespace Scheduler
{
	/**
	 * @ingroup math 
	 * @{
	 * @defgroup integer_power Compile-time integer power 
	 *
	 * This is a collection of templates for integer power calculation at the compile-time
	 *
	 * @{
	 */

	template <unsigned long long N, unsigned int P, int Odd = (P & 1)>
	struct IntegerPower;

	template <unsigned long long N, unsigned int P>
	struct IntegerPower<N, P, 0>
	{
		static const unsigned long long val = IntegerPower<N * N, (P / 2)>::val;
	};

	template <unsigned long long N, unsigned int P>
	struct IntegerPower<N, P, 1>
	{
		static const unsigned long long val = N * IntegerPower<N, (P - 1)>::val;
	};

	template <unsigned long long N>
	struct IntegerPower<N, 0, 0>
	{
		static const unsigned long long val = 1;
	};

	/**
	 * @}
	 * @}
	 */
}