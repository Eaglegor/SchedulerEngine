#include "OneRelocateTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/MoveWorkStop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Utils/InvariantWorkStopsList.h>

namespace Scheduler
{
	OneRelocateTSPSolver::OneRelocateTSPSolver():
	logger(LoggingManager::getLogger("OneRelocateTSPSolver"))
	{
	}

    void OneRelocateTSPSolver::optimize(Schedule& schedule) const
	{
		if (!schedule_cost_function) return;

		for (Run& run : schedule.getRuns())
		{
			optimize(run);
		}
	}

	void OneRelocateTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "OneRelocateTSPSolver::optimize(Run&)", logger);
		
		if (!schedule_cost_function) return;
		
		if(run.getWorkStops().empty()) return;
		
		InvariantWorkStopsList stops(run.getWorkStops());
		
		Cost best_cost = schedule_cost_function->calculateCost(run.getSchedule());
		
		SceneEditor editor;
		bool changed = true;
		while (changed) {
			changed = false;
            for (auto stop_it1 = stops.begin(); stop_it1 != std::prev(stops.end()); ++stop_it1) {
                for (auto stop_it2 = std::next(stop_it1); stop_it2 != stops.end(); ++stop_it2) {
					if (std::next(stop_it1) == stop_it2) continue;
                    editor.performAction<MoveWorkStop>(run, stops.getRunWorkStopIterator(run, stop_it1), stops.getRunWorkStopIterator(run, stop_it2));
					Cost cost = schedule_cost_function->calculateCost(run.getSchedule());
					if (cost < best_cost) {
						best_cost = cost;
						changed = true;
						editor.commit();
						stops = run.getWorkStops();
					}
					else {
						editor.rollbackAll();
					}

				}
			}
		}
	}

    void OneRelocateTSPSolver::setScheduleCostFunction(const ScheduleCostFunction& cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
