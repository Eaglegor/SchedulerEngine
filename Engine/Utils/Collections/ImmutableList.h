#pragma once

#include <list>

namespace Scheduler
{
	/// Represents the unmodifiable list
	template <typename T>
	using ImmutableList = const std::list<T>;
}