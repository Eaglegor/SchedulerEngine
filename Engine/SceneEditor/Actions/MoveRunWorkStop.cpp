#include "MoveRunWorkStop.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>
#include <assert.h>

namespace Scheduler
{
	MoveRunWorkStop::MoveRunWorkStop(ConstRunIterator run_iterator, ConstWorkStopIterator from, ConstWorkStopIterator to) :
		backend(run_iterator, from, std::next(from), to)
	{
	}

	void MoveRunWorkStop::perform()
	{
		backend.perform();
	}

	void MoveRunWorkStop::rollback()
	{
		backend.rollback();
	}
}
