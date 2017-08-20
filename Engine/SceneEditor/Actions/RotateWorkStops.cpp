#include "RotateWorkStops.h"

#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <assert.h>

namespace Scheduler
{
	RotateWorkStops::RotateWorkStops(Run& run, Run::ConstWorkStopIterator first, Run::ConstWorkStopIterator middle, Run::ConstWorkStopIterator last)
	    : backend(run, first, middle, last)
	{
	}

	void RotateWorkStops::perform( )
	{
		backend.perform( );
	}

	void RotateWorkStops::rollback( )
	{
		backend.rollback( );
	}
}
