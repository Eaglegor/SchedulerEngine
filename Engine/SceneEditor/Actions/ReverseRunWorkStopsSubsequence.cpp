#include "ReverseRunWorkStopsSubsequence.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <assert.h>

namespace Scheduler
{
	ReverseWorkStopsSubsequence::ReverseWorkStopsSubsequence(Run* r, Stop* start_stop, Stop* end_stop):
		schedule(r->getSchedule()),
		run_index(determine_run_index(r)),
		start_index(determine_stop_index(start_stop)),
		end_index(determine_stop_index(end_stop))
	{
	}

	ReverseWorkStopsSubsequence::ReverseWorkStopsSubsequence(Run* r, size_t start_index, size_t end_index):
		schedule(r->getSchedule()),
		run_index(determine_run_index(r)),
		start_index(start_index),
		end_index(end_index)
	{
	}

	void ReverseWorkStopsSubsequence::perform()
	{
		if (start_index == end_index) return;

		Run* r = schedule->getRuns()[run_index];

		int ia = start_index;
		int ib = end_index;

		while(ia < ib)
		{
			const Operation* oa = r->getWorkStops()[ia]->getOperation();
			const Operation* ob = r->getWorkStops()[ib]->getOperation();

			r->replaceWorkOperationAt(ia, ob);
			r->replaceWorkOperationAt(ib, oa);

			++ia;
			--ib;
		}
		
	}

	void ReverseWorkStopsSubsequence::rollback()
	{
		perform();
	}

	size_t ReverseWorkStopsSubsequence::determine_run_index(Run* run)
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

	size_t ReverseWorkStopsSubsequence::determine_stop_index(Stop* stop)
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
}
