#include "TheBestTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/ScheduleStateUtils.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <iostream>

namespace Scheduler
{
    TheBestTSPSolver::TheBestTSPSolver()
	{
	}

    void TheBestTSPSolver::optimize(Schedule* schedule) const
	{
        if (!schedule_cost_function) return;
        sequentialOptimize(schedule);
	}

	void TheBestTSPSolver::optimize(Run* run) const
	{
        if (!schedule_cost_function) return;
        sequentialOptimize(run);
	}

    void TheBestTSPSolver::addTSPSolver(TSPSolver* aTSPSolver)
	{
        this->tsp_solvers.push_back(aTSPSolver);
	}

    void TheBestTSPSolver::setScheduleCostFunction(ScheduleCostFunction *cost_function)
    {
        schedule_cost_function = cost_function;
    }

    void TheBestTSPSolver::sequentialOptimize(Schedule* schedule) const
    {
        TemporarySchedule best_schedule;
        Cost best_cost;
        size_t idx = 0;
        for (auto solver : this->tsp_solvers) {

            TemporarySchedule temporary_schedule = schedule->getScene()->createTemporaryScheduleCopy(schedule);
            solver->optimize(temporary_schedule.get());
            const Cost cost = schedule_cost_function->calculateCost(temporary_schedule.get());
            if (cost < best_cost || !best_schedule) {
                best_cost = cost;
                best_schedule = std::move(temporary_schedule);
                std::cout << "select new schedule at " << idx << " for cost " << best_cost.getValue() << std::endl;
            }
            ++idx;
        }

        ScheduleStateUtils::copyState(best_schedule.get(), schedule);
    }

    void TheBestTSPSolver::sequentialOptimize(Run* run) const
    {
        TemporarySchedule best_schedule;
        Schedule* run_schedule = run->getSchedule();
        auto it = std::find(run_schedule->getRuns().begin(), run_schedule->getRuns().end(), run);
        const size_t run_index = std::distance(run_schedule->getRuns().begin(), it);
        Cost best_cost;
        for (auto solver : this->tsp_solvers) {
            TemporarySchedule temporary_schedule = run_schedule->getScene()->createTemporaryScheduleCopy(run_schedule);
            Run* temporary_run = temporary_schedule->getRuns().at(run_index);
            solver->optimize(temporary_run);
            Cost cost = schedule_cost_function->calculateCost(temporary_schedule.get());
            if (cost < best_cost || !best_schedule) {
                best_cost = cost;
                best_schedule.reset(temporary_schedule.get());
            }
        }

        ScheduleStateUtils::copyState(best_schedule.get(), run_schedule);
    }

    void TheBestTSPSolver::concurrentOptimize(Schedule* schedule) const
    {
        ;
    }

    void TheBestTSPSolver::concurrentOptimize(Run* schedule) const
    {
        ;
    }
}
