#include "MoveRunWorkStopsSubsequence.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>

#include <assert.h>

namespace Scheduler
{
	MoveRunWorkStopsSubsequence::MoveRunWorkStopsSubsequence(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop, WorkStopIterator new_position) :
		schedule((*run_iterator)->getSchedule()),
		irun(std::distance(schedule->getRuns().begin(), run_iterator)),
		istart_stop(std::distance((*run_iterator)->getWorkStops().begin(), start_stop)),
		iend_stop(std::distance((*run_iterator)->getWorkStops().begin(), end_stop)),
		inew_position(std::distance((*run_iterator)->getWorkStops().begin(), new_position)),
		rotate_command(
			run_iterator,
			inew_position < istart_stop ? new_position : start_stop,
			inew_position <= istart_stop ? start_stop : end_stop,
			iend_stop < inew_position ? new_position : end_stop
		)
	{
		assert(std::distance(new_position, start_stop) >= 0 || std::distance(end_stop, new_position) > 0);
	}

	void MoveRunWorkStopsSubsequence::perform()
	{
		rotate_command.perform();
	}

	void MoveRunWorkStopsSubsequence::rollback()
	{
		rotate_command.rollback();
	}
}
