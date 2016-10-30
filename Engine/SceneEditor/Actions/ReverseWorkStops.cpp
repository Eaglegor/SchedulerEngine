#include "ReverseWorkStops.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <assert.h>

namespace Scheduler
{
	ReverseWorkStops::ReverseWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator last) :
		run(run),
		first(first),
		last(last)
	{
	}

	void ReverseWorkStops::perform()
	{
		new_first = std::prev(last);
		run.reverseWorkStops(first, last);
	}

	void ReverseWorkStops::rollback()
	{
		run.reverseWorkStops(new_first, last);
	}
}
