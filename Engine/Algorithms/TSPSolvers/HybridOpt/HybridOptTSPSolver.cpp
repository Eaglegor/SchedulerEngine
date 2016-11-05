#include "HybridOptTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/Actions/MoveWorkStop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Utils/InvariantWorkStopsList.h>

namespace Scheduler
{
	HybridOptTSPSolver::HybridOptTSPSolver():
	logger(LoggingManager::getLogger("HybridOptTSPSolver"))
	{
	}

    void HybridOptTSPSolver::optimize(Schedule& schedule) const
	{
		if (!schedule_cost_function) return;

		for(Run& run : schedule.getRuns())
		{
			optimize(run);
		}
	}

	void HybridOptTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "HybridOptTSPSolver::optimize(Run&)", logger);
		
		if (!schedule_cost_function) return;
		
		if(run.getWorkStops().empty()) return;
		
		InvariantWorkStopsList stops(run.getWorkStops());

        Cost best_cost = schedule_cost_function->calculateCost(run.getSchedule());
		
        bool changed = true;
		
        SceneEditor editor;
		
        while (changed) {
            changed = false;
            for (auto stop_it1 = stops.begin(); stop_it1 != std::prev(stops.end()); ++stop_it1) {
                for (auto stop_it2 = std::next(stop_it1); stop_it2 != stops.end(); ++stop_it2) {
					
					Patch best_patch;
					Cost current_best_cost(std::numeric_limits<Cost::value_type>::max());
					
					Patch reverse_patch = editor.createPatch();
                    reverse_patch.performAction<ReverseWorkStops>(run, stops.getRunWorkStopIterator(run, stop_it1), stops.getRunWorkStopIterator(run, std::next(stop_it2)));
                    Cost cost = schedule_cost_function->calculateCost(run.getSchedule());
					reverse_patch.hold();
					if(cost < current_best_cost)
					{
						current_best_cost = cost;
						best_patch = std::move(reverse_patch);
					}

					Patch swap_patch = editor.createPatch();
                    swap_patch.performAction<SwapWorkStops>(run, stops.getRunWorkStopIterator(run, stop_it1), stops.getRunWorkStopIterator(run, stop_it2));
                    cost = schedule_cost_function->calculateCost(run.getSchedule());
					swap_patch.hold();
					if(cost < current_best_cost)
					{
						current_best_cost = cost;
						best_patch = std::move(swap_patch);
					}

					Patch move_patch = editor.createPatch();
                    move_patch.performAction<MoveWorkStop>(run, stops.getRunWorkStopIterator(run, stop_it1), stops.getRunWorkStopIterator(run, stop_it2));
                    cost = schedule_cost_function->calculateCost(run.getSchedule());
					move_patch.hold();
					if(cost < current_best_cost)
					{
						current_best_cost = cost;
						best_patch = std::move(move_patch);
					}

                    if (current_best_cost < best_cost) {
                        best_cost = current_best_cost;
                        changed = true;
						
						editor.applyPatch(std::move(best_patch));
						editor.commit();
						
						stops = run.getWorkStops();
                    }
                    else
					{
						editor.abortPatching();
					}
                }
            }
        }
	}

	void HybridOptTSPSolver::setScheduleCostFunction(const ScheduleCostFunction& cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
