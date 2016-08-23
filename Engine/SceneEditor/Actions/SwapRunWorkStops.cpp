#include "SwapRunWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
	SwapRunWorkStops::SwapRunWorkStops(RunIterator run_iterator, WorkStopIterator start_stop, WorkStopIterator end_stop):
		schedule((*run_iterator)->getSchedule()),
		run(run_iterator),
		first(start_stop),
		second(end_stop)
	{
	}

	void SwapRunWorkStops::perform()
	{
		(*run)->swapWorkStops(first, second);
	}

	void SwapRunWorkStops::rollback()
	{
		(*run)->swapWorkStops(first, second);
	}
}
