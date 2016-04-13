#include "ListTemperatureScheduler.h"
#include <algorithm>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/TemporarySchedule.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{

std::vector<Cost> TemperatureScheduler::create_initial_costs(Run* originalRun, ScheduleCostFunction* schedule_cost_function, size_t length, unsigned long long seed_value)
{
    std::vector<Cost> costs;

    TemporarySchedule temporary_schedule = originalRun->getSchedule()->getScene()->createTemporaryScheduleCopy(originalRun->getSchedule());
    auto original_run_iter = std::find(originalRun->getSchedule()->getRuns().begin(), originalRun->getSchedule()->getRuns().end(), originalRun);
    Run* run = temporary_schedule->getRuns().at(std::distance(originalRun->getSchedule()->getRuns().begin(), original_run_iter));
    auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
    const auto &stops = run->getWorkStops();

    std::mt19937_64 random_engine(seed_value);
    std::uniform_int_distribution<> index_distribution(0, stops.size() - 1);
    std::uniform_real_distribution<> float_distribution(0.f, 1.f);

    Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

    for (size_t number_of_iterations = 0; number_of_iterations < length; ++number_of_iterations) {
        SceneEditor editor;
        size_t i = 0;
        size_t j = 0;
        while (i == j) {
            i = index_distribution(random_engine);
            j = index_distribution(random_engine);
        }
        if (i > j) {
            std::swap(i,  j);
        }

        const float random_value = float_distribution(random_engine);random_value < 2.f / 3;
        if (random_value < 1.f / 3) {
            editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
        } else if (random_value < 2.f / 3){
            editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + i, stops.begin() + j + 1);
        } else {
            editor.performAction<MoveRunWorkStop>(run_iter, stops.begin() + i, stops.begin() + j);
        }
        const Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
        if (cost < best_cost) {
            best_cost = cost;
        } else {
            editor.rollbackAll();
        }
        costs.push_back(cost);
    }

    return costs;
}

}
