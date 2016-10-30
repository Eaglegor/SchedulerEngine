#include "MoveWorkStop.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>
#include <assert.h>

namespace Scheduler
{
	MoveWorkStop::MoveWorkStop(Run& run, Run::ConstWorkStopIterator from, Run::ConstWorkStopIterator to):
	backend(run, from, std::next(from), to),
	action_needed(from != to && to != std::next(from))
	{
	}

	MoveWorkStop::MoveWorkStop(Run& from_run, Run::ConstWorkStopIterator from_pos, Run& to_run, Run::ConstWorkStopIterator to_pos):
	backend(from_run, from_pos, std::next(from_pos), to_run, to_pos),
	action_needed(from_pos != to_pos && to_pos != std::next(from_pos))
	{
	}
	
	void MoveWorkStop::perform()
	{
		if(action_needed) backend.perform();
	}

	void MoveWorkStop::rollback()
	{
		if(action_needed) backend.rollback();
	}
}
