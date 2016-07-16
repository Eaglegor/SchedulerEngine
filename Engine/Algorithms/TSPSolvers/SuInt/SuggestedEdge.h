#pragma once
#include <cstddef>

namespace Scheduler
{
	struct SuggestedEdge
	{
		size_t from_index;
		size_t to_index;
	};
}