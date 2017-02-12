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

DefaultTemperatureScheduler::DefaultTemperatureScheduler(float ap0, float ap_end, std::size_t aiterations_number) : p0(ap0), p_end(ap_end), iterations_number(aiterations_number)
{}

float DefaultTemperatureScheduler::getTemperature() const
{
    return current_temperature;
}

bool DefaultTemperatureScheduler::isFinish() const
{
    return iterations_number == 0 || current_temperature <= 0.f;
}

const char* DefaultTemperatureScheduler::getName() const
{
    return "Default";
}

void DefaultTemperatureScheduler::adapt (Cost delta, float random)
{
    ;
}

void DefaultTemperatureScheduler::changeTemperature()
{
    current_temperature *= k;
    if (iterations_number >= 0) {
        --iterations_number;
    }
}

void DefaultTemperatureScheduler::initialize(Run& run, const ScheduleCostFunction& cost_function)
{
    const auto costs = create_initial_costs(run, cost_function, 120);
    Cost first_cost = costs.front();
    float sum_diff = 0.f;
    for (auto it = std::next(costs.begin()); it < costs.end(); ++it) {
        sum_diff += fabs((first_cost - *it).getValue());
    }
    const float avr = sum_diff / (costs.size() - 1);
    current_temperature = -avr / p0;
    k = std::pow(-avr / p_end / current_temperature, (float)1 / iterations_number);
}

TemperatureScheduler* DefaultTemperatureScheduler::clone () const
{
    return new DefaultTemperatureScheduler(*this);
}

}
