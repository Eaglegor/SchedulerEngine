#pragma once

namespace Scheduler
{
	/** 
	 * @ingroup memory_manager
	 * 
	 * @brief This namespace contains some predefined object sizes to be used with pool memory allocators
	 */
	namespace Pool
	{
		/**
		 * @brief Size of tiny object
		 */
		constexpr static size_t TINY_OBJECT   = 48;

		/**
		 * @brief Size of small object
		 */
		constexpr static size_t SMALL_OBJECT  = 80;

		/**
		 * @brief Size of medium object
		 */
		constexpr static size_t MEDIUM_OBJECT = 200;

		/**
		 * @brief Size of large object
		 */
		constexpr static size_t LARGE_OBJECT  = 400;

		/**
		 * @brief Size of huge object
		 */
		constexpr static size_t HUGE_OBJECT   = 1000;
	}
}