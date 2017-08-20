#include "SimpleTwoOptTSPSolver.h"
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Utils/InvariantWorkStopsList.h>

namespace Scheduler
{
	SimpleTwoOptTSPSolver::SimpleTwoOptTSPSolver( )
	    : logger(LoggingManager::getLogger("SimpleTwoOptTSPSolver"))
	{
	}

	void SimpleTwoOptTSPSolver::optimize(Schedule& schedule) const
	{
		if(!schedule_cost_function) return;

		for(Run& run : schedule.getRuns( ))
		{
			optimize(run);
		}
	}

	void SimpleTwoOptTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "SimpleTwoOptTSPSolver::optimize(Run&)", logger);

		if(!schedule_cost_function) return;

		if(run.getWorkStops( ).empty( )) return;

		const auto& stops = run.getWorkStops( );

		Cost best_cost = schedule_cost_function->calculateCost(run.getSchedule( ));

		bool changed = true;
		while(changed)
		{
			changed = false;
			for(auto stop_it1 = stops.begin( ); stop_it1 != stops.end( ); ++stop_it1)
			{
				for(auto stop_it2 = std::next(stop_it1); stop_it2 != stops.end( ); ++stop_it2)
				{
					SceneEditor editor;
					editor.performAction<ReverseWorkStops>(run, stop_it1, std::next(stop_it2));
					Cost cost = schedule_cost_function->calculateCost(run.getSchedule( ));
					if(cost < best_cost)
					{
						best_cost = cost;
						changed   = true;
						editor.commit( );
						std::swap(stop_it1, stop_it2); // stop_it1 is now pointing to the end of the reversed chain - so we need to swap iterators to make it point to our next stop
					}
					else
					{
						editor.rollbackAll( );
					}
				}
			}
		}
	}

	void SimpleTwoOptTSPSolver::setScheduleCostFunction(const ScheduleCostFunction& cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
