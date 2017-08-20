#include "SwapWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <assert.h>

namespace Scheduler
{
	SwapWorkStops::SwapWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator second)
	    : run(run),
	      first(first),
	      second(second)
	{
	}

	void SwapWorkStops::perform( )
	{
		run.swapWorkStops(first, second);
	}

	void SwapWorkStops::rollback( )
	{
		run.swapWorkStops(first, second);
	}
}
