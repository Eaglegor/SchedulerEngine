#pragma once

#include <vector>

namespace Scheduler
{
	template<typename T>
	using ImmutableVector = const std::vector<T>;
}