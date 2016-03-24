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

	SwapRunWorkStops::SwapRunWorkStops(Run* r, size_t a_index, size_t b_index):
		schedule(r->getSchedule()),
		ir(determine_run_index(r)),
		ia(a_index),
		ib(b_index)
	{
	}

	void SwapRunWorkStops::perform()
	{
		if (ia == ib) return;

		Run* r = schedule->getRuns()[ir];

		const Operation* oa = *(r->getWorkStops()[ia]->getOperations().begin());
		const Operation* ob = *(r->getWorkStops()[ib]->getOperations().begin());

		r->replaceWorkOperationAt(ia, ob);
		r->replaceWorkOperationAt(ib, oa);
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
