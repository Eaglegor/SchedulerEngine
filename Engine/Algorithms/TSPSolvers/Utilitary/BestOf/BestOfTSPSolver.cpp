#include "BestOfTSPSolver.h"
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
#include <Engine/SceneManager/ScheduleVariant.h>


namespace Scheduler
{
    BestOfTSPSolver::BestOfTSPSolver():
    concurrency_enabled(true),
    logger(LoggingManager::getLogger("BestOfTSPSolver"))
	{
	}

    void BestOfTSPSolver::optimize(Schedule& schedule) const
	{
        if (!schedule_cost_function) return;
		if(concurrency_enabled) concurrentOptimize(schedule);
		else sequentialOptimize(schedule);
	}

	void BestOfTSPSolver::optimize(Run& run) const
	{
        if (!schedule_cost_function) return;
        if(concurrency_enabled) concurrentOptimize(run);
		else sequentialOptimize(run);
	}

    void BestOfTSPSolver::addTSPSolver(const TSPSolver& aTSPSolver)
	{
        this->tsp_solvers.emplace_back(aTSPSolver);
	}

    void BestOfTSPSolver::setScheduleCostFunction(const ScheduleCostFunction &cost_function)
    {
        schedule_cost_function = cost_function;
    }

    void BestOfTSPSolver::sequentialOptimize(Schedule& schedule) const
    {
		TraceableSection("BestOfTSPSolver::sequentialOptimize(Schedule&)", logger);
		
        ScheduleVariant best_variant;
        Cost best_cost;
        std::size_t idx = 0;
        for (const TSPSolver& solver : tsp_solvers) {
            ScheduleVariant variant = schedule;
            solver.optimize(variant.getSchedule().get());
            const Cost cost = schedule_cost_function->calculateCost(variant.getSchedule().get());
            if (cost < best_cost || !best_variant) {
                best_cost = cost;
                best_variant = std::move(variant);
            }
            ++idx;
        }

        best_variant.apply();
    }

    void BestOfTSPSolver::sequentialOptimize(Run& run) const
    {
		TraceableSection("BestOfTSPSolver::sequentialOptimize(Run&)", logger);
		
        ScheduleVariant best_variant;
        Schedule& run_schedule = run.getSchedule();
        auto it = std::find(run_schedule.getRuns().begin(), run_schedule.getRuns().end(), run);
        const std::size_t run_index = std::distance(run_schedule.getRuns().begin(), it);
        Cost best_cost;
        for (const TSPSolver& solver : this->tsp_solvers) {
            ScheduleVariant variant = run_schedule;
            Run& temporary_run = variant.getSchedule()->getRuns().at(run_index);
            solver.optimize(temporary_run);
            Cost cost = schedule_cost_function->calculateCost(variant.getSchedule().get());
            if (cost < best_cost || !best_variant) {
                best_cost = cost;
                best_variant = std::move(variant);
            }
        }

        best_variant.apply();
    }

    void BestOfTSPSolver::concurrentOptimize(Schedule& schedule) const
    {
        TraceableSection("BestOfTSPSolver::concurrentOptimize(Schedule&)", logger);
		
		std::vector<ScheduleVariant> variants;
        for (std::size_t idx = 0; idx < tsp_solvers.size(); ++idx) {
            variants.emplace_back(schedule);
        }
		
		Cost best_cost;
        ScheduleVariant best_variant;
		
        #pragma omp parallel for
        for (int idx = 0; idx < this->tsp_solvers.size(); ++idx) {
			logger.trace("Starting solver: {}", this->tsp_solvers.at(idx).get().getName() );
            const TSPSolver& solver = this->tsp_solvers.at(idx);
			ScheduleVariant &variant = variants.at(idx);
            Schedule& schedule = variant.getSchedule().get();
            solver.optimize(schedule);
			logger.trace("Solver '{}' finished work", this->tsp_solvers.at(idx).get().getName() );
			
			const Cost cost = schedule_cost_function->calculateCost(schedule);
			
			logger.debug("Resulting cost by '{}': {}", this->tsp_solvers.at(idx).get().getName(), cost.getValue() );
			
			#pragma omp critical
			{
				if (cost < best_cost || !best_variant) {
					best_cost = cost;
					best_variant = std::move(variant);
					logger.debug("Better than best result found by '{}'", this->tsp_solvers.at(idx).get().getName(), cost.getValue() );
				}
			}
        }

        best_variant.apply();
    }

    void BestOfTSPSolver::concurrentOptimize(Run& run) const
    {
		TraceableSection("BestOfTSPSolver::concurrentOptimize(Run&)", logger);
		
		std::size_t run_index = std::distance<Schedule::ConstRunIterator>(run.getSchedule().getRuns().begin(), run.getSchedule().findRun(run));
		
		std::vector<ScheduleVariant> variants;

        for (std::size_t idx = 0; idx < tsp_solvers.size(); ++idx) {
            variants.emplace_back(run.getSchedule());
        }
        
        Cost best_cost;
        ScheduleVariant best_variant;
        
        #pragma omp parallel for
        for (int idx = 0; idx < this->tsp_solvers.size(); ++idx) {
            const TSPSolver& solver = this->tsp_solvers.at(idx);
			ScheduleVariant& variant = variants.at(idx);
            Schedule& schedule = variant.getSchedule().get();
            solver.optimize(*(std::next(schedule.getRuns().begin(), run_index)));
        
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
    
    void BestOfTSPSolver::setConcurrencyEnabled(bool value)
	{
		this->concurrency_enabled = value;
	}
}
