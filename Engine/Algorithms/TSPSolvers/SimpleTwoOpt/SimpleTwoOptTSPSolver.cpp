#include "SimpleTwoOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/Utils/Collections/PositionPreservingLinkedPointersListWrapper.h>

namespace Scheduler
{
	SimpleTwoOptTSPSolver::SimpleTwoOptTSPSolver():
		schedule_cost_function(nullptr)
	{
	}

    void SimpleTwoOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

		for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

	void SimpleTwoOptTSPSolver::optimize(Run* run) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

		if(run->getWorkStops().empty()) return;
		
		PositionPreservingLinkedPointersListWrapper<Run::WorkStopsList> stops(run->getWorkStops());

		auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
        bool changed = true;
        while (changed) {
            changed = false;
            for (auto stop_it1 = stops.begin(); stop_it1 != stops.end(); ++stop_it1) {
                for (auto stop_it2 = std::next(stop_it1); stop_it2 != stops.end(); ++stop_it2) {
                    SceneEditor editor;
                    editor.performAction<ReverseWorkStopsSubsequence>(run_iter, *stop_it1, *std::next(stop_it2));
                    Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
                    if (cost < best_cost) {
                        best_cost = cost;
                        changed = true;
						editor.commit();
						stops.update();
                    } else {
                        editor.rollbackAll();
                    }
                }
            }
        }
	}

	void SimpleTwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
