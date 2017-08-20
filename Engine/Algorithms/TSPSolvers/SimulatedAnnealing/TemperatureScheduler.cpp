#include "ListTemperatureScheduler.h"
#include "SolutionGeneratorClassic.h"
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{

	std::vector<Cost> TemperatureScheduler::create_initial_costs(Run& run, const ScheduleCostFunction& schedule_cost_function, std::size_t length)
	{
		std::vector<Cost> costs;
		Cost best_cost = schedule_cost_function.calculateCost(run.getSchedule( ));
		SolutionGeneratorClassic solution_generator(run);
		solution_generator.enableMutation(SolutionGenerator::MutationType::BlockInsert);
		solution_generator.enableMutation(SolutionGenerator::MutationType::BlockReverse);
		solution_generator.enableMutation(SolutionGenerator::MutationType::VertexInsert);
		solution_generator.enableMutation(SolutionGenerator::MutationType::VertexSwap);

		for(size_t i = 0; i < length; ++i)
		{
			const std::size_t checkpoint = solution_generator.checkpoint( );
			solution_generator.neighbour( );
			const Cost cost = schedule_cost_function.calculateCost(run.getSchedule( ));
			if(cost < best_cost)
			{
				best_cost = cost;
			}
			else
			{
				solution_generator.rollbackTo(checkpoint);
			}
			costs.push_back(cost);
		}
		solution_generator.discard( );

		return costs;
	}
}
