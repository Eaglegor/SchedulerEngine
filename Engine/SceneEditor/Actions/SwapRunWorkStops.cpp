#include "SwapRunWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>
#include <assert.h>

namespace Scheduler
{
	SwapRunWorkStops::SwapRunWorkStops(Run* r, Stop* a, Stop* b):
		schedule(r->getSchedule()),
		ir(determine_run_index(r)),
		ia(determine_stop_index(a)),
		ib(determine_stop_index(b))
	{

	}

	void SwapRunWorkStops::perform()
	{
		if (ia == ib) return;

		Run* r = schedule->getRuns()[ir];

		const Operation* oa = *(r->getWorkStops()[ia]->getOperations().begin());
		const Operation* ob = *(r->getWorkStops()[ib]->getOperations().begin());

		r->unallocateWorkOperationAt(std::max(ia, ib));
		r->unallocateWorkOperationAt(std::min(ia, ib));

		if(ia > ib)
		{
			r->allocateWorkOperation(oa, ib);
			r->allocateWorkOperation(ob, ia);
		}
		else
		{
			r->allocateWorkOperation(ob, ia);
			r->allocateWorkOperation(oa, ib);
		}
	}

	void SwapRunWorkStops::rollback()
	{
		perform();
	}

	size_t SwapRunWorkStops::determine_stop_index(Stop* stop)
	{
		Run* run = stop->getRun();
		for (size_t i = 0; i < run->getWorkStops().size(); ++i)
		{
			if (run->getWorkStops()[i] == stop) return i;
		}

		// We are not allowed to go there
		assert(false);
		return -1;
	}

	size_t SwapRunWorkStops::determine_run_index(Run* run)
	{
		Schedule* schedule = run->getSchedule();
		for (size_t i = 0; i < schedule->getRuns().size(); ++i)
		{
			if (schedule->getRuns()[i] == run) return i;
		}

		// We are not allowed to go there
		assert(false);
		return -1;
	}

}
