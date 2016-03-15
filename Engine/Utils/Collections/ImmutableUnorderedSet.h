#pragma once

#include <unordered_set>

namespace Scheduler
{
	template<typename T>
	using ImmutableUnorderedSet = const std::unordered_set<T>;
}