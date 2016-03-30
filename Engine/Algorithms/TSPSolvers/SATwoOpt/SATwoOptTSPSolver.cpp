#include "SATwoOptTSPSolver.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>

namespace Scheduler
{
    SATwoOptTSPSolver::SATwoOptTSPSolver():
        schedule_cost_function(nullptr),
        acceptance_function(nullptr),
        temperature_function(nullptr)
	{
	}

    void SATwoOptTSPSolver::optimize(Schedule* schedule) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!acceptance_function) return;
        if (!temperature_function) return;

		for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

    void SATwoOptTSPSolver::optimize(Run* run) const
	{
		if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!acceptance_function) return;
        if (!temperature_function) return;

        temperature_function->reset();
        const auto &stops = run->getWorkStops();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());
        bool finish = false;
        while (!finish) {
            for (size_t i = 0; i < (stops.size() - 1) && !finish; ++i) {
                for (size_t j = i + 1; j < stops.size() && !finish; ++j) {
                    SceneEditor editor;
                    editor.performAction<ReverseWorkStopsSubsequence>(run, i, j + 1);
                    Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
                    if (cost < best_cost || acceptance_function->isAccepting(cost - best_cost, temperature_function->getTemperature())) {
                        best_cost = cost;
                    } else {
                        editor.rollbackAll();
                    }
                    temperature_function->changeTemperature();
                    finish = temperature_function->isFinish();
                }
            }
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

    void SATwoOptTSPSolver::setTemperatureFunction(TemperatureFunction *temperature_function)
    {
        this->temperature_function = temperature_function;
    }
}
