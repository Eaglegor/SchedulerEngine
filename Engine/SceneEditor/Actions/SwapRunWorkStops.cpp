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
		ir(std::distance(schedule->getRuns().begin(), run_iterator)),
		ia(std::distance((*run_iterator)->getWorkStops().begin(), start_stop)),
		ib(std::distance((*run_iterator)->getWorkStops().begin(), end_stop))
	{
	}

	void SwapRunWorkStops::perform()
	{
		if (ia == ib) return;

		Run* r = schedule->getRuns()[ir];

		ActionsImpl::swapRunWorkStops(r, ia, ib);
	}

	void SwapRunWorkStops::rollback()
	{
		perform();
	}
}
