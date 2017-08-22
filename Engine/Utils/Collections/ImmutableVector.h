#pragma once

#include <vector>

namespace Scheduler
{
	/// Represents the unmodifiable vector
	template <typename T>
	using ImmutableVector = const std::vector<T>;
}