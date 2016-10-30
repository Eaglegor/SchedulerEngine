#include "ListTemperatureScheduler.h"
#include <algorithm>
#include <mutex>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include "SolutionGenerator.h"

namespace Scheduler
{

std::vector<Cost> TemperatureScheduler::create_initial_costs(Run& run, const ScheduleCostFunction& schedule_cost_function, std::size_t length)
{
    std::vector<Cost> costs;
    Cost best_cost = schedule_cost_function.calculateCost(run.getSchedule());
    SolutionGeneratorClassic solution_generator(run);
    solution_generator.enableMutation(SolutionGenerator::MutationType::BlockInsert);
    solution_generator.enableMutation(SolutionGenerator::MutationType::BlockReverse);
    solution_generator.enableMutation(SolutionGenerator::MutationType::VertexInsert);
    solution_generator.enableMutation(SolutionGenerator::MutationType::VertexSwap);

    for (size_t number_of_iterations = 0; number_of_iterations < length; ++number_of_iterations) {
        const std::size_t checkpoint = solution_generator.checkpoint();
        solution_generator.neighbour();
        const Cost cost = schedule_cost_function.calculateCost(run.getSchedule());
        if (cost < best_cost) {
            best_cost = cost;
        } else {
            solution_generator.rollbackTo(checkpoint);
        }
        costs.push_back(cost);
    }
    solution_generator.discard();

    return costs;
}

}
