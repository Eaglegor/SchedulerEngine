#include "MoveRunWorkStopsSubsequence.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>

#include <assert.h>

namespace Scheduler
{
	MoveRunWorkStopsSubsequence::MoveRunWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator end_stop, ConstWorkStopIterator new_position) :
		schedule((*run_iterator)->getSchedule()),
		run(run_iterator),
		from_run((*start_stop)->getRun()),
		first(start_stop),
		last(end_stop),
		new_position(new_position),
		action_needed(end_stop != new_position && std::prev(end_stop) != new_position && new_position != start_stop)
	{
	}

	void MoveRunWorkStopsSubsequence::perform()
	{
		if(!action_needed) return;
		(*run)->spliceWorkStops(new_position, first, last);
	}

	void MoveRunWorkStopsSubsequence::rollback()
	{
		if(!action_needed) return;
		from_run->spliceWorkStops(last, first, new_position);
	}
}
