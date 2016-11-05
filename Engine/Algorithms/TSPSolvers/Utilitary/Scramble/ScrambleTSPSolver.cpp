#include "ScrambleTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/ScrambleWorkStops.h>

namespace Scheduler
{
	ScrambleTSPSolver::ScrambleTSPSolver():
	logger(LoggingManager::getLogger("ScrambleTSPSolver"))
	{
	}
	
	const char* ScrambleTSPSolver::getName() const
	{
		return staticGetName();
	}
	
	const char* ScrambleTSPSolver::staticGetName()
	{
		return "Scramble";
	}

	void ScrambleTSPSolver::optimize(Schedule& schedule) const
	{
		TRACEABLE_SECTION(__optimize__, "ScrambleTSPSolver::optimize(Schedule&)", logger);
		
		for(Run& r : schedule.getRuns())
		{
			optimize(r);
		}
	}

	void ScrambleTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "ScrambleTSPSolver::optimize(Run&)", logger);
		
		SceneEditor editor;
		editor.performAction<ScrambleWorkStops>(run, run.getWorkStops().begin(), run.getWorkStops().end());
		editor.commit();
	}
}