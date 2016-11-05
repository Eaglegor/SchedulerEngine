#include "IterativeTSPSolver.h"
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
	IterativeTSPSolver::IterativeTSPSolver(const TSPSolver& backend_solver, const ScheduleCostFunction& schedule_cost_function):
	backend_solver(backend_solver),
	schedule_cost_function(schedule_cost_function),
	iterations_count(1),
	logger(LoggingManager::getLogger("IterativeTSPSolver"))
	{
	}

	void IterativeTSPSolver::optimize(Schedule& schedule) const
	{
		TRACEABLE_SECTION(__optimize__, "IterativeTSPSolver::optimize(Schedule&)", logger);
		
		Cost best_cost = schedule_cost_function.calculateCost(schedule);
		ScheduleVariant best_variant = schedule;
		
		for(int i = 0; i < iterations_count; ++i)
		{
			LOG_TRACE(logger, "Iteration {} / {}", i, iterations_count);
			backend_solver.optimize(schedule);
			Cost new_cost = schedule_cost_function.calculateCost(schedule);
			LOG_DEBUG(logger, "New cost: {} (best: {})", new_cost.getValue(), best_cost.getValue());
			if(new_cost < best_cost) 
			{
				LOG_DEBUG(logger, "Solution accepted");
				best_cost = new_cost;
				best_variant = schedule;
			}
		}
		
		best_variant.apply();
	}

	void IterativeTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "IterativeTSPSolver::optimize(Run&)", logger);
		
		Cost best_cost = schedule_cost_function.calculateCost(run.getSchedule());
		ScheduleVariant best_variant = run.getSchedule();
		
		for(int i = 0; i < iterations_count; ++i)
		{
			LOG_TRACE(logger, "Iteration {} / {}", i, iterations_count);
			backend_solver.optimize(run);
			Cost new_cost = schedule_cost_function.calculateCost(run.getSchedule());
			LOG_DEBUG(logger, "New cost: {} (best: {})", new_cost.getValue(), best_cost.getValue());
			if(new_cost < best_cost) {
				LOG_DEBUG(logger, "Solution accepted");
				best_cost = new_cost;
				best_variant = run.getSchedule();
			}
		}
		
		best_variant.apply();
	}
	
	void IterativeTSPSolver::setIterationsCount(std::size_t iterations_count)
	{
		this->iterations_count = iterations_count;
	}

}
