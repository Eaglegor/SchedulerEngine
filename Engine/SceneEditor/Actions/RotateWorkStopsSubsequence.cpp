#include "RotateWorkStopsSubsequence.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include "ActionsImpl.h"
#include <assert.h>

namespace Scheduler
{
    RotateWorkStopsSubsequence::RotateWorkStopsSubsequence(ConstRunIterator run_iterator, ConstWorkStopIterator start_stop, ConstWorkStopIterator middle_stop, ConstWorkStopIterator end_stop) :
		backend(run_iterator, start_stop, middle_stop, end_stop)
	{
	}

	void RotateWorkStopsSubsequence::perform()
	{
		backend.perform();
	}

	void RotateWorkStopsSubsequence::rollback()
	{
        backend.rollback();
	}
}
