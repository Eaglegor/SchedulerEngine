#include "MoveRunWorkStop.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>
#include <assert.h>

namespace Scheduler
{
	MoveRunWorkStop::MoveRunWorkStop(Run* r, WorkStop* from, WorkStop* to):
		schedule(r->getSchedule()),
		irun(determine_run_index(r)),
		ifrom(determine_stop_index(from)),
		ito(determine_stop_index(to))
	{

	}

	MoveRunWorkStop::MoveRunWorkStop(Run* r, size_t from_index, size_t to_index):
		schedule(r->getSchedule()),
		irun(determine_run_index(r)),
		ifrom(from_index),
		ito(to_index)
	{
	}

	void MoveRunWorkStop::perform()
	{
		if (ifrom == ito) return;

		Run* r = schedule->getRuns()[irun];

		const Operation* ofrom = r->getWorkStops()[ifrom]->getOperation();

		size_t di = ifrom < ito ? 1 : -1;

		for (size_t i = ifrom; i != ito; i += di)
		{
			r->replaceWorkOperationAt(i, r->getWorkStops()[i + di]->getOperation());
		}

		r->replaceWorkOperationAt(ito, ofrom);
	}

	void MoveRunWorkStop::rollback()
	{
		size_t temp = ifrom;
		ifrom = ito;
		ito = temp;

		perform();

		ito = ifrom;
		ifrom = temp;
	}

	size_t MoveRunWorkStop::determine_stop_index(WorkStop* stop)
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

	size_t MoveRunWorkStop::determine_run_index(Run* run)
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
