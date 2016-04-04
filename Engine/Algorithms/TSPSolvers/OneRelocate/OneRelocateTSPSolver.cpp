#include "OneRelocateTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <iostream>

namespace Scheduler
{
	OneRelocateTSPSolver::OneRelocateTSPSolver():
		schedule_cost_function(nullptr)
	{
	}

    void OneRelocateTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

		for (Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

	void OneRelocateTSPSolver::optimize(Run* run) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't

		const auto &stops = run->getWorkStops();
		Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
		bool changed = true;
		while (changed) {
			changed = false;
			for (auto stop_it1 = stops.begin(); stop_it1 != stops.end(); ++stop_it1) {
				for (auto stop_it2 = stop_it1; stop_it2 != stops.end(); ++stop_it2) {
					SceneEditor editor;
					editor.performAction<MoveRunWorkStop>(run, *stop_it1, *stop_it2);
					Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
					if (cost < best_cost) {
						best_cost = cost;
						changed = true;
					}
					else {
						editor.rollbackAll();
					}

				}
			}
		}
	}

    void OneRelocateTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
	{
		this->schedule_cost_function = cost_function;
	}
}
