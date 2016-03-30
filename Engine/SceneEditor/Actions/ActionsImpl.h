#pragma once

#include <cstddef>

namespace Scheduler
{
	class Run;
	namespace ActionsImpl
	{
		void swapRunWorkStops(Run* r, size_t ia, size_t ib);
	}
}