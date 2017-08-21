#pragma once

namespace Scheduler
{
	/** 
	 * @brief This namespace contains some predefined object sizes to be used with pool memory allocators
	 */
	namespace Pool
	{
		constexpr static size_t TINY_OBJECT   = 48;
		constexpr static size_t SMALL_OBJECT  = 80;
		constexpr static size_t MEDIUM_OBJECT = 200;
		constexpr static size_t LARGE_OBJECT  = 400;
		constexpr static size_t HUGE_OBJECT   = 1000;
	}
}