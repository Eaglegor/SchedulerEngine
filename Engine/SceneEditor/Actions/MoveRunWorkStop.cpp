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
        irun(std::distance<ImmutableVector<Run*>::const_iterator>(schedule->getRuns().begin(), run_iterator)),
        ifrom(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), from)),
        ito(std::distance<ImmutableVector<WorkStop*>::const_iterator>((*run_iterator)->getWorkStops().begin(), to))
	{

	}

	void MoveRunWorkStop::perform()
	{
		if (ifrom == ito) return;

		Run* r = schedule->getRuns()[irun];

		const Operation* ofrom = r->getWorkStops()[ifrom]->getOperation();

        int di = ifrom < ito ? 1 : -1;

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
}
