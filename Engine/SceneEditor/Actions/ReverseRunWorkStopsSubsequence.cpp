#include "ReverseRunWorkStopsSubsequence.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
	ReverseWorkStopsSubsequence::ReverseWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator end_stop) :
		schedule((*run_iterator)->getSchedule()),
		run(run_iterator),
		first(start_stop),
		last(end_stop)
	{
	}

	void ReverseWorkStopsSubsequence::perform()
	{
		new_first = std::prev(last);
		(*run)->reverseWorkStops(first, last);
	}

	void ReverseWorkStopsSubsequence::rollback()
	{
		(*run)->reverseWorkStops(new_first, last);
	}
}
