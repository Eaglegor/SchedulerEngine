#include "SATwoOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>

namespace Scheduler
{
    SATwoOptTSPSolver::SATwoOptTSPSolver():
        schedule_cost_function(nullptr),
        acceptance_function(nullptr)
	{
	}

    void SATwoOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!acceptance_function) return;

		for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

    void SATwoOptTSPSolver::optimize(Run* run) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!acceptance_function) return;

        const auto &stops = run->getWorkStops();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
        bool changed = true;
        unsigned long long counter = 0;
        while (changed) {
            changed = false;
            for (auto stop_it1 = stops.begin(); stop_it1 != stops.end() - 1; ++stop_it1) {
                for (auto stop_it2 = stop_it1 + 1; stop_it2 != stops.end(); ++stop_it2) {
                    SceneEditor editor;
                    editor.performAction<SwapRunWorkStops>(run, *stop_it1, *stop_it2);
                    editor.performAction<ReverseWorkStopsSubsequence>(run, *(stop_it1 + 1), *(stop_it2));
                    Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
                    if (cost < best_cost || acceptance_function->isAccepting(cost - best_cost, counter)) {
                        best_cost = cost;
                        changed = true;
                    } else {
                        editor.rollbackAll();
                    }

                }
            }
            ++counter;
        }
	}

    void SATwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}

    void SATwoOptTSPSolver::setAcceptanceFunction(AcceptanceFunction *acceptance_function)
    {
        this->acceptance_function = acceptance_function;
    }
}
