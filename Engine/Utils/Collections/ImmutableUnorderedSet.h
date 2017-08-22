#pragma once

#include <unordered_set>

namespace Scheduler
{
	/// Represents the unmodifiable hash set
	template <typename T>
	using ImmutableUnorderedSet = const std::unordered_set<T>;
}