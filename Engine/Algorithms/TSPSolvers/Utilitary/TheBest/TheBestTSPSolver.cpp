#include "TheBestTSPSolver.h"
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Utils/SceneCloner.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneManager/ScheduleVariant.h>


namespace Scheduler
{
    TheBestTSPSolver::TheBestTSPSolver():
    concurrency_enabled(true),
    logger(LoggingManager::getLogger("TheBestTSPSolver"))
	{
	}

    void TheBestTSPSolver::optimize(Schedule* schedule) const
	{
        if (!schedule_cost_function) return;
		if(concurrency_enabled) concurrentOptimize(schedule);
		else sequentialOptimize(schedule);
	}

	void TheBestTSPSolver::optimize(Run* run) const
	{
        if (!schedule_cost_function) return;
        if(concurrency_enabled) concurrentOptimize(run);
		else sequentialOptimize(run);
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
		SIMPLE_LOG_TRACE(logger, "Starting sequential schedule optimization");
		
        ScheduleVariant best_variant;
        Cost best_cost;
        std::size_t idx = 0;
        for (auto solver : this->tsp_solvers) {
            ScheduleVariant variant = schedule;
            solver->optimize(variant.getSchedule());
            const Cost cost = schedule_cost_function->calculateCost(variant.getSchedule());
            if (cost < best_cost || !best_variant) {
                best_cost = cost;
                best_variant = std::move(variant);
            }
            ++idx;
        }

        best_variant.apply();
    }

    void TheBestTSPSolver::sequentialOptimize(Run* run) const
    {
		SIMPLE_LOG_TRACE(logger, "Starting sequential run optimization");
		
        ScheduleVariant best_variant;
        Schedule* run_schedule = run->getSchedule();
        auto it = std::find(run_schedule->getRuns().begin(), run_schedule->getRuns().end(), run);
        const size_t run_index = std::distance(run_schedule->getRuns().begin(), it);
        Cost best_cost;
        for (auto solver : this->tsp_solvers) {
            ScheduleVariant variant = run_schedule;
            Run* temporary_run = variant.getSchedule()->getRuns().at(run_index);
            solver->optimize(temporary_run);
            Cost cost = schedule_cost_function->calculateCost(variant.getSchedule());
            if (cost < best_cost || !best_variant) {
                best_cost = cost;
                best_variant = std::move(variant);
            }
        }

        best_variant.apply();
    }

    void TheBestTSPSolver::concurrentOptimize(Schedule* schedule) const
    {
        SIMPLE_LOG_TRACE(logger, "Starting concurrent schedule optimization");
		
		std::vector<ScheduleVariant> variants;
        for (size_t idx = 0; idx < tsp_solvers.size(); ++idx) {
            variants.emplace_back(schedule);
        }
		
		Cost best_cost;
        ScheduleVariant best_variant;
		
        #pragma omp parallel for
        for (int idx = 0; idx < this->tsp_solvers.size(); ++idx) {
			LOG_TRACE(logger, "Starting solver: {}", this->tsp_solvers.at(idx)->getName() );
            TSPSolver* solver = this->tsp_solvers.at(idx);
			ScheduleVariant &variant = variants.at(idx);
            Schedule* schedule = variant.getSchedule();
            solver->optimize(schedule);
			LOG_TRACE(logger, "Solver '{}' finished work", this->tsp_solvers.at(idx)->getName() );
			
			const Cost cost = schedule_cost_function->calculateCost(schedule);
			
			LOG_DEBUG(logger, "Resulting cost by '{}': {}", this->tsp_solvers.at(idx)->getName(), cost.getValue() );
			
			#pragma omp critical
			{
				if (cost < best_cost || !best_variant) {
					best_cost = cost;
					best_variant = std::move(variant);
					LOG_DEBUG(logger, "Better than best result found by '{}'", this->tsp_solvers.at(idx)->getName(), cost.getValue() );
				}
			}
        }

        best_variant.apply();
    }

    void TheBestTSPSolver::concurrentOptimize(Run* run) const
    {
		SIMPLE_LOG_TRACE(logger, "Starting concurrent run optimization");
		
		ImmutableVector<Run*> &runs = run->getSchedule()->getRuns();
		std::ptrdiff_t run_index = std::distance(runs.begin(), std::find(runs.begin(), runs.end(), run));
		
		std::vector<ScheduleVariant> variants;

        for (std::size_t idx = 0; idx < tsp_solvers.size(); ++idx) {
            variants.emplace_back(run->getSchedule());
        }
        
        Cost best_cost;
        ScheduleVariant best_variant;
        
        #pragma omp parallel for
        for (int idx = 0; idx < this->tsp_solvers.size(); ++idx) {
            TSPSolver* solver = this->tsp_solvers.at(idx);
			ScheduleVariant& variant = variants.at(idx);
            Schedule* schedule = variant.getSchedule();
            solver->optimize(*(std::next(schedule->getRuns().begin(), run_index)));
        
			const Cost cost = schedule_cost_function->calculateCost(schedule);
			
			#pragma omp critical
			{
				if (cost < best_cost || !best_variant) {
					best_cost = cost;
					best_variant = std::move(variant);
				}
			}
		}
		
        best_variant.apply();
    }
    
    void TheBestTSPSolver::setConcurrencyEnabled(bool value)
	{
		this->concurrency_enabled = value;
	}
}
