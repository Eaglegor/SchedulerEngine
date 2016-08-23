#include "ActionsImpl.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Operation.h>

namespace Scheduler
{
	namespace ActionsImpl
	{
		void swapRunWorkStops(Run* r, size_t ia, size_t ib)
		{
			auto i1 = std::next(r->getWorkStops().begin(), ia);
			auto i2 = std::next(r->getWorkStops().begin(), ib);
			
			r->swapWorkStops(i1, i2);
		}
	}
}