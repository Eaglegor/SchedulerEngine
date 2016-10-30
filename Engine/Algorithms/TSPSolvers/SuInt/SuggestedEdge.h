#pragma once
#include <cstddef>

namespace Scheduler
{
	struct SuggestedEdge
	{
		std::size_t from_index;
		std::size_t to_index;
	};
}