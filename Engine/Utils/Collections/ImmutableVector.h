#pragma once

#include <vector>

namespace Scheduler
{
	/**
	 * @ingroup collections
	 * 
	 * @brief Represents the unmodifiable vector
	 */
	template <typename T>
	using ImmutableVector = const std::vector<T>;
}