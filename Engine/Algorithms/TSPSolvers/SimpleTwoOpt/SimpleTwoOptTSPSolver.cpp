#include "SimpleTwoOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>

namespace Scheduler
{
	SimpleTwoOptTSPSolver::SimpleTwoOptTSPSolver():
		schedule_cost_function(nullptr)
	{
	}

    void SimpleTwoOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

        const auto &runs = schedule->getRuns();
        for (auto run_it = runs.begin(); run_it != runs.end(); ++run_it) {
            optimize(schedule, std::distance(run_it, runs.end() - 1));
        }
	}

	void SimpleTwoOptTSPSolver::optimize(Schedule* schedule, size_t run_index) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

        const auto &runs = schedule->getRuns();
        const auto run_ptr = runs.at(run_index);
        const auto &stops = run_ptr->getWorkStops();
        Cost best_cost = schedule_cost_function->calculateCost(run_ptr->getSchedule());
        for (auto stop_it1 = stops.begin(); stop_it1 != stops.end() - 1; ++stop_it1) {
            for (auto stop_it2 = stop_it1 + 1; stop_it2 != stops.end(); ++stop_it2) {
                SceneEditor editor;
                editor.performAction<SwapRunWorkStops>(run_ptr, *stop_it1, *stop_it2);
                editor.performAction<ReverseWorkStopsSubsequence>(run_ptr, *(stop_it1 + 1), *(stop_it2 - 1));
                Cost cost = schedule_cost_function->calculateCost(schedule);
                if (cost > best_cost) {
                    best_cost = cost;
                } else {
                    editor.rollbackAll();
                }

            }
        }
	}

	void SimpleTwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
