#pragma once

#include <list>

namespace Scheduler
{
	template <typename T>
	using ImmutableList = const std::list<T>;
}