#pragma once

#include <list>

namespace Scheduler
{
	/**
	 * @brief Represents the unmodifiable list
	 */ 
	template <typename T>
	using ImmutableList = const std::list<T>;
}