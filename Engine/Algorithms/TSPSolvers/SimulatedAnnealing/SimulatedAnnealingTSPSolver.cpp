#include "SimulatedAnnealingTSPSolver.h"
#include <random>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/TemporarySchedule.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStop.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include "TemperatureScheduler.h"

namespace Scheduler
{
    SimulatedAnnealingTSPSolver::SimulatedAnnealingTSPSolver():
        schedule_cost_function(nullptr),
        temperature_scheduler(nullptr),
        seed_value(5489ull),
        use_adaptive_ops(true),
        markov_scale(1.f)
    {
    }

    void SimulatedAnnealingTSPSolver::optimize(Schedule* schedule) const
    {
        if (!schedule_cost_function) return;
        if (!temperature_scheduler) return;

        size_t r = 0;
        for(Run* run : schedule->getRuns())
        {
            optimize(run);
        }
    }

    void SimulatedAnnealingTSPSolver::optimize(Run* run) const
    {
        if (!schedule_cost_function) return;
        if (!temperature_scheduler) return;       

        auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();

        std::random_device rd;
        auto nseed_value = rd();
        std::mt19937_64 random_engine(nseed_value);

        std::uniform_int_distribution<> index_distribution(0, stops.size() - 1);
        std::uniform_real_distribution<> float_distribution(0.f, 1.f);

        temperature_scheduler->initialize(run, schedule_cost_function, nseed_value);
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        size_t number_of_iterations = 0;
        size_t number_of_swap = 0;
        size_t number_of_reverse = 0;
        size_t number_of_move = 0;
        size_t good_swap = 0;
        size_t good_reverse = 0;
        size_t good_move = 0;

        float swap_interval = 1.f / 3;
        float reverse_interval = 1.f / 3;
        float move_interval = 1.f / 3;

        float markov_scale_ = markov_scale;

        const size_t light_limit = 100 * 100 * 3;
        const size_t medium_limit = 100 * 100 * 3 * 6;
        size_t M_ = std::nearbyint(stops.size() * markov_scale_);
        if (stops.size() < 100 &&
            M_ * stops.size() > light_limit) {
            M_ = light_limit / stops.size();
        }

        if (stops.size() >= 100 &&
            M_ * stops.size() > medium_limit) {
            M_ = medium_limit / stops.size();
        }

        const size_t M = std::max(M_, (size_t)1);

        while (!temperature_scheduler->isFinish()) {
            for (size_t m = 0; m < M; ++m) {
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

                const bool swap_op = (random_value < swap_interval);
                const bool reverse_op = !swap_op && (random_value < (swap_interval + reverse_interval));
                const bool move_op = !swap_op && !reverse_op;

                if (reverse_op) {
                    editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + i, stops.begin() + j + 1);
                    ++number_of_reverse;
                } else if (swap_op) {
                    editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
                    ++number_of_swap;
                } else if (move_op) {
                    editor.performAction<MoveRunWorkStop>(run_iter, stops.begin() + i, stops.begin() + j);
                    ++number_of_move;
                } else {
                    assert(false);
                }

                Cost cost = schedule_cost_function->calculateCost(run->getSchedule());

                if (cost < best_cost) {
                    best_cost = cost;
                    if (reverse_op) {
                        ++good_reverse;
                    } else if (swap_op) {
                        ++good_swap;
                    } else if (move_op) {
                        ++good_move;
                    }
                } else if (acceptance(cost - best_cost, float_distribution(random_engine))) {
                    best_cost = cost;
                } else {
                    editor.rollbackAll();
                }

                ++number_of_iterations;
                if (use_adaptive_ops && (number_of_iterations % 1000) == 0) {
                    const float good_swap_choice = static_cast<float>(good_swap) / number_of_swap;
                    const float good_reverse_choice = static_cast<float>(good_reverse) / number_of_reverse;
                    const float good_move_choice = static_cast<float>(good_move) / number_of_move;

                    swap_interval = good_swap_choice / (good_swap_choice + good_reverse_choice + good_move_choice);
                    reverse_interval = good_reverse_choice / (good_swap_choice + good_reverse_choice + good_move_choice);
                    move_interval = good_move_choice / (good_swap_choice + good_reverse_choice + good_move_choice);
                }
            }

            temperature_scheduler->changeTemperature();
        }
    }

    bool SimulatedAnnealingTSPSolver::acceptance(Cost delta, float random) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature_scheduler->getTemperature());
        if (random <= normalized_value) {
            temperature_scheduler->adapt(-delta.getValue() / std::log(random));
            return true;
        }
        return false;
    }

    void SimulatedAnnealingTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
    {
        this->schedule_cost_function = cost_function;
    }

    void SimulatedAnnealingTSPSolver::setTemperatureScheduler(TemperatureScheduler *temperature_function)
    {
        this->temperature_scheduler = temperature_function;
    }

    void SimulatedAnnealingTSPSolver::seed(unsigned long long value)
    {
        this->seed_value = value;
    }

    void SimulatedAnnealingTSPSolver::setUseAdaptiveOps(bool useAdaptiveOps)
    {
        this->use_adaptive_ops = useAdaptiveOps;
    }

    void SimulatedAnnealingTSPSolver::setMarkovScale(float markovScale)
    {
        this->markov_scale = markovScale;
    }
}
