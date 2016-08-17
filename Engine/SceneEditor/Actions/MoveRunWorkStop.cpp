#include "MoveRunWorkStop.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>
#include <assert.h>

namespace Scheduler
{
	MoveRunWorkStop::MoveRunWorkStop(RunIterator run_iterator, WorkStopIterator from, WorkStopIterator to) :
		schedule((*run_iterator)->getSchedule()),
		irun(std::distance(schedule->getRuns().begin(), run_iterator)),
		ifrom(std::distance((*run_iterator)->getWorkStops().begin(), from)),
		ito(std::distance((*run_iterator)->getWorkStops().begin(), to)),
		move_command(run_iterator, from, std::next(from), to)
	{
	}

	void MoveRunWorkStop::perform()
	{
		move_command.perform();
	}

	void MoveRunWorkStop::rollback()
	{
		move_command.rollback();
	}
}
