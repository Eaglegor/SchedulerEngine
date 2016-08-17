#include "HybridOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>

namespace Scheduler
{
	HybridOptTSPSolver::HybridOptTSPSolver():
		schedule_cost_function(nullptr)
	{
	}

    void HybridOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

		for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

	void HybridOptTSPSolver::optimize(Run* run) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

        const auto &stops = run->getWorkStops();
		if (stops.empty()) return;

		auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
        bool changed = true;
        SceneEditor editor;
        while (changed) {
            changed = false;
            for (auto stop_it1 = stops.begin(); stop_it1 != std::prev(stops.end()); ++stop_it1) {
                for (auto stop_it2 = std::next(stop_it1); stop_it2 != stops.end(); ++stop_it2) {
                    editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stop_it1, std::next(stop_it2));
                    const Cost reverse_cost = schedule_cost_function->calculateCost(run->getSchedule());
                    editor.rollbackAll();

                    editor.performAction<SwapRunWorkStops>(run_iter, stop_it1, stop_it2);
                    const Cost swap_cost = schedule_cost_function->calculateCost(run->getSchedule());
                    editor.rollbackAll();

                    editor.performAction<MoveRunWorkStop>(run_iter, stop_it1, stop_it2);
                    const Cost move_cost = schedule_cost_function->calculateCost(run->getSchedule());
                    editor.rollbackAll();

                    Cost cost = std::min(reverse_cost, swap_cost);
                    cost = std::min(cost, move_cost);

                    if (cost < best_cost) {
                        best_cost = cost;
                        changed = true;
                        if (cost == reverse_cost) {
                            editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stop_it1, std::next(stop_it2));
                        } else if (cost == swap_cost) {
                            editor.performAction<SwapRunWorkStops>(run_iter, stop_it1, stop_it2);
                        } else {
                            editor.performAction<MoveRunWorkStop>(run_iter, stop_it1, stop_it2);
                        }
                        editor.commit();
                    }
                }
            }
        }
	}

	void HybridOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
