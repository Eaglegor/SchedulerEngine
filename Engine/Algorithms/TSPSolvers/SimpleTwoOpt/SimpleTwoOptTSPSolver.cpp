#include "SimpleTwoOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>

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

        const auto &stops = run->getWorkStops();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
        bool changed = true;
        while (changed) {
            changed = false;
            for (size_t i = 0; i < stops.size() - 1; ++i) {
                for (size_t j = i + 1; j < stops.size(); ++j) {
                    SceneEditor editor;
                    editor.performAction<ReverseWorkStopsSubsequence>(run, i, j + 1);
                    Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
                    if (cost < best_cost) {
                        best_cost = cost;
                        changed = true;
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
