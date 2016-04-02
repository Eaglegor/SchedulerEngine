#include "SATwoOptTSPSolver.h"
#include <random>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>

namespace Scheduler
{
    SATwoOptTSPSolver::SATwoOptTSPSolver():
        schedule_cost_function(nullptr),
        temperature_scheduler(nullptr)
    {
    }

    void SATwoOptTSPSolver::optimize(Schedule* schedule) const
    {
        if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!temperature_scheduler) return;

        for(Run* run : schedule->getRuns())
        {
            optimize(run);
        }
    }

    void SATwoOptTSPSolver::optimize(Run* run) const
    {
        if (!schedule_cost_function) return; // We don't have a metric to optimize - so we can't
        if (!temperature_scheduler) return;

        auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();
        std::default_random_engine random_engine;
        std::uniform_int_distribution<> index_distribution(0, stops.size() - 1);
        std::uniform_real_distribution<> float_distribution(0.f, 1.f);

        temperature_scheduler->reset();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        while (!temperature_scheduler->isFinish()) {
            SceneEditor editor;
            size_t i = index_distribution(random_engine);
            size_t j = index_distribution(random_engine);
            editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
            Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
            if (cost < best_cost || isAccepting(cost - best_cost, float_distribution(random_engine))) {
                best_cost = cost;
            } else {
                editor.rollbackAll();
            }
            temperature_scheduler->changeTemperature();
        }
    }

    void SATwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
    {
        this->schedule_cost_function = cost_function;
    }

    void SATwoOptTSPSolver::setTemperatureScheduler(TemperatureScheduler *temperature_function)
    {
        this->temperature_scheduler = temperature_function;
    }

    bool SATwoOptTSPSolver::isAccepting(Cost delta, float random) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature_scheduler->getTemperature());
        return random <= normalized_value;
    }
}
