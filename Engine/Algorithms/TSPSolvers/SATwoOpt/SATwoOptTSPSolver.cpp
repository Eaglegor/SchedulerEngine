#include "SATwoOptTSPSolver.h"
#include <random>
#include <map>
#include <iostream>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>

namespace Scheduler
{
    SATwoOptTSPSolver::SATwoOptTSPSolver():
        schedule_cost_function(nullptr),
        temperature_scheduler(nullptr),
        type(SimulatedAnnealingType::Default)
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

        switch (type) {
        case SimulatedAnnealingType::Greedy:
            greedy_optimize(run);
            break;
        case SimulatedAnnealingType::Default:
        default:
            default_optimize(run);
            break;

        }
    }

    void SATwoOptTSPSolver::default_optimize(Run* run) const
    {
        auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();

        std::default_random_engine random_engine;
        std::uniform_int_distribution<> index_distribution(0, stops.size() - 1);
        std::uniform_real_distribution<> float_distribution(0.f, 1.f);

        temperature_scheduler->reset();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        size_t number_of_iterations = 0;
        size_t number_of_swap = 0;
        size_t number_of_reverse = 0;
        size_t good_reverse = 0;
        size_t good_swap = 0;
        float choice_limit = 0.5f;

        while (!temperature_scheduler->isFinish()) {
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

            const float random_value = float_distribution(random_engine);
            const bool choice_swap = (random_value < choice_limit);
            if (choice_swap) {
                editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
                ++number_of_swap;
            } else {
                editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + i, stops.begin() + j + 1);
                ++number_of_reverse;
            }
            const Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
            if (cost < best_cost) {
                if (choice_swap) {
                    ++good_swap;
                } else {
                    ++good_reverse;
                }
                best_cost = cost;
            } else if (acceptance(cost - best_cost, float_distribution(random_engine))) {
                best_cost = cost;
            } else {
                editor.rollbackAll();
            }
            temperature_scheduler->changeTemperature();
            ++number_of_iterations;
            if ((number_of_iterations % 1000) == 0) {
                const float good_swap_choice = static_cast<float>(good_swap) / number_of_swap;
                const float good_reverse_choice = static_cast<float>(good_reverse) / number_of_reverse;

                choice_limit = good_swap_choice / (good_swap_choice + good_reverse_choice);
                choice_limit = std::max(0.05f, choice_limit);
                choice_limit = std::min(0.95f, choice_limit);
            }
        }
    }

    void SATwoOptTSPSolver::greedy_optimize(Run* run) const
    {
        auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();

        std::default_random_engine random_engine;
        std::uniform_int_distribution<> index_distribution(0, stops.size() - 1);
        std::uniform_real_distribution<> float_distribution(0.f, 1.f);

        temperature_scheduler->reset();
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        const size_t t_greedy = stops.size() / 10;

        while (!temperature_scheduler->isFinish()) {
            SceneEditor editor;
            size_t g = 0;
            bool is_better_cost = false;
            Cost greedy_best_cost;
            size_t greedy_best_i = 0;
            size_t greedy_best_j = 0;
            bool greedy_best_op_is_reverse = false;

            while (g < t_greedy && !is_better_cost) {
                size_t i = 0;
                size_t j = 0;
                while (i == j) {
                    i = index_distribution(random_engine);
                    j = index_distribution(random_engine);
                }
                if (i > j) {
                    std::swap(i,  j);
                }

                if (g % 2) {
                    editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + i, stops.begin() + j + 1);
                } else {
                    editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
                }

                const Cost cost = schedule_cost_function->calculateCost(run->getSchedule());

                if (cost < best_cost) {
                    best_cost = cost;
                    is_better_cost = true;
                } else {
                    if (g == 0 || cost < greedy_best_cost) {
                        greedy_best_cost = cost;
                        greedy_best_i = i;
                        greedy_best_j = j;
                        greedy_best_op_is_reverse = g % 2;
                    }
                    editor.rollbackAll();
                    ++g;
                }
            }

            if (!is_better_cost) {
                if (acceptance(greedy_best_cost - best_cost, float_distribution(random_engine))) {
                    best_cost = greedy_best_cost;
                    if (greedy_best_op_is_reverse) {
                        editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + greedy_best_i, stops.begin() + greedy_best_j + 1);
                    } else {
                        editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + greedy_best_i, stops.begin() + greedy_best_j);
                    }
                }
            }

            temperature_scheduler->changeTemperature();
        }
    }

    bool SATwoOptTSPSolver::acceptance(Cost delta, float random) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature_scheduler->getTemperature());
        return random <= normalized_value;
    }

    void SATwoOptTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
    {
        this->schedule_cost_function = cost_function;
    }

    void SATwoOptTSPSolver::setTemperatureScheduler(TemperatureScheduler *temperature_function)
    {
        this->temperature_scheduler = temperature_function;
    }

    void SATwoOptTSPSolver::setType(SimulatedAnnealingType type)
    {
        this->type = type;
    }
}
