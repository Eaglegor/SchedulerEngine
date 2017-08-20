#include "RotateTSPSolver.h"
#include <Engine/SceneEditor/Actions/RotateWorkStops.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	RotateTSPSolver::RotateTSPSolver( )
	    : ratio(0),
	      logger(LoggingManager::getLogger("RotateTSPSolver"))
	{
	}

	const char* RotateTSPSolver::getName( ) const
	{
		return staticGetName( );
	}

	const char* RotateTSPSolver::staticGetName( )
	{
		return "Rotate";
	}

	void RotateTSPSolver::optimize(Schedule& schedule) const
	{
		TRACEABLE_SECTION(__optimize__, "RotateTSPSolver::optimize(Schedule&)", logger);

		for(Run& r : schedule.getRuns( ))
		{
			optimize(r);
		}
	}

	void RotateTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "RotateTSPSolver::optimize(Run&)", logger);

		if(run.getWorkStops( ).empty( )) return;

		std::size_t middle_index = static_cast<std::size_t>((run.getWorkStops( ).size( ) - 1) * ratio);

		LOG_DEBUG(logger, "Ratio: {}, count: {}, middle: {}", ratio, run.getWorkStops( ).size( ), middle_index);

		if(middle_index == 0)
		{
			LOG_DEBUG(logger, "Middle index = 0 - rotation not needed");
			return;
		}

		SceneEditor editor;
		editor.performAction<RotateWorkStops>(run, run.getWorkStops( ).begin( ), std::next(run.getWorkStops( ).begin( ), middle_index), run.getWorkStops( ).end( ));
		editor.commit( );
	}

	void RotateTSPSolver::setRatio(float ratio)
	{
		assert(ratio >= 0 && ratio <= 1);
		this->ratio = ratio;
	}
}