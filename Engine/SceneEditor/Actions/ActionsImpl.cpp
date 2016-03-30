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
			const Operation* oa = r->getWorkStops()[ia]->getOperation();
			const Operation* ob = r->getWorkStops()[ib]->getOperation();

			r->replaceWorkOperationAt(ia, ob);
			r->replaceWorkOperationAt(ib, oa);
		}
	}
}