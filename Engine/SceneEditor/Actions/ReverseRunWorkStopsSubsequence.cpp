#include "ReverseRunWorkStopsSubsequence.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
	ReverseWorkStopsSubsequence::ReverseWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop) :
		schedule((*run_iterator)->getSchedule()),
		run_index(std::distance(schedule->getRuns().begin(), run_iterator)),
		start_index(std::distance((*run_iterator)->getWorkStops().begin(), start_stop)),
		end_index(std::distance((*run_iterator)->getWorkStops().begin(), end_stop))
	{
		assert(run_index >= 0);
		assert(run_index < schedule->getRuns().size());
		assert(start_index >= 0);
		assert(start_index < (*run_iterator)->getWorkStops().size());
		assert(end_index > start_index);
		assert(end_index <= (*run_iterator)->getWorkStops().size());
	}

	void ReverseWorkStopsSubsequence::perform()
	{
		if (start_index == end_index) return;

		Run* r = schedule->getRuns()[run_index];

		int ia = start_index;
		int ib = end_index - 1;

		while(ia < ib)
		{
			ActionsImpl::swapRunWorkStops(r, ia, ib);

			++ia;
			--ib;
		}
		
	}

	void ReverseWorkStopsSubsequence::rollback()
	{
		perform();
	}
}
