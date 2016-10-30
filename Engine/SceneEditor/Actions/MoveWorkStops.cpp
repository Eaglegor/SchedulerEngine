#include "MoveWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Schedule.h>
#include <algorithm>

#include <assert.h>

namespace Scheduler
{
	MoveWorkStops::MoveWorkStops(Run& from_run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last, Run& to_run, Run::ConstWorkStopIterator pos):
	from_run(from_run),
	first(first),
	last(last),
	to_run(to_run),
	pos(pos),
	action_needed(last != pos && std::prev(last) != pos && first != pos)
	{
		assert(first != pos);
	}

	MoveWorkStops::MoveWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last, Run::ConstWorkStopIterator pos):
	from_run(run),
	first(first),
	last(last),
	to_run(run),
	pos(pos),
	action_needed(last != pos)
	{
		assert(first != pos);
	}

	
	void MoveWorkStops::perform()
	{
		if(!action_needed) return;
		to_run.spliceWorkStops(pos, from_run, first, last);
	}

	void MoveWorkStops::rollback()
	{
		if(!action_needed) return;
		from_run.spliceWorkStops(last, to_run, first, pos);
	}
}
