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
        markov_chain_lenght_scale(1.f)
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

    size_t SimulatedAnnealingTSPSolver::calculate_markov_chain_length (Run* run) const
    {
        const auto &stops = run->getWorkStops();

        float markov_scale_ = markov_chain_lenght_scale;

        size_t M_ = std::nearbyint(stops.size() * markov_scale_);

        if (limits.size()) {
            const size_t stops_size = stops.size();
            auto limit_it = std::find_if(limits.begin(), limits.end(), [stops_size] (const std::pair<size_t,size_t> & lv) {
                return stops_size < lv.first;
            });
            if (limit_it == limits.end()) {
                limit_it = std::prev(limits.end());
            }


            if (M_ * stops.size() > limit_it->second) {
                M_ = limit_it->second / stops.size();
            }
        }

        return std::max(M_, (size_t)1);
    }

    void SimulatedAnnealingTSPSolver::optimize(Run* run) const
    {
        if (!schedule_cost_function) return;
        if (!temperature_scheduler) return;

        const auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();

        if (stops.size() <= 1) {
            return;
        }

        random_shuffle(run);

        temperature_scheduler->initialize(run, schedule_cost_function);
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::mt19937_64 random_engine2(random_device());
        std::uniform_int_distribution<size_t> index_distribution(0, stops.size() - 1);
        std::uniform_real_distribution<float> float_distribution(0.f, 1.f);
        SceneEditor scene_editor;
        const size_t M = calculate_markov_chain_length(run);
        while (!temperature_scheduler->isFinish()) {
            for (size_t m = 0; m < M; ++m) {
                size_t i = 0;
                size_t j = 0;
                while (i == j) {
                    i = index_distribution(random_engine2);
                    j = index_distribution(random_engine2);
                }
                const float random_value = float_distribution(random_engine);
                if (random_value < 1.f / 3) {
                    scene_editor.performAction<MoveRunWorkStop>(run_iter, stops.begin() + i, stops.begin() + j);
                } else if (random_value < 2.f / 3) {
                    const size_t ri = std::min(i, j);
                    const size_t rj = std::max(i, j) + 1;
                    scene_editor.performAction<ReverseWorkStopsSubsequence>(run_iter, stops.begin() + ri, stops.begin() + rj);
                } else {
                    scene_editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + j);
                }

                const Cost cost = schedule_cost_function->calculateCost((*run_iter)->getSchedule());

                if (cost < best_cost) {
                    best_cost = cost;
                    scene_editor.commit();
                } else {
                    const float random_value = float_distribution(random_engine2);
                    if (acceptance(cost - best_cost, random_value)) {
                        temperature_scheduler->adapt((best_cost - cost).getValue() / std::log(random_value));
                        best_cost = cost;
                        scene_editor.commit();
                    } else {
                        scene_editor.rollbackAll();
                    }
                }
            }
            temperature_scheduler->changeTemperature();
        }
    }

    void SimulatedAnnealingTSPSolver::random_shuffle (Run* run) const
    {
        const auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const auto &stops = run->getWorkStops();
        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_int_distribution<size_t> shuffle_distribution;
        SceneEditor scene_editor;
        for (size_t i = stops.size() - 1; i > 0; --i) {
            scene_editor.performAction<SwapRunWorkStops>(run_iter, stops.begin() + i, stops.begin() + shuffle_distribution(random_engine, std::uniform_int_distribution<size_t>::param_type(0, i)));
        }
    }

    bool SimulatedAnnealingTSPSolver::acceptance(Cost delta, float random) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature_scheduler->getTemperature());
        return random <= normalized_value;
    }

    void SimulatedAnnealingTSPSolver::setScheduleCostFunction(ScheduleCostFunction* cost_function)
    {
        this->schedule_cost_function = cost_function;
    }

    void SimulatedAnnealingTSPSolver::setTemperatureScheduler(TemperatureScheduler *temperature_function)
    {
        this->temperature_scheduler = temperature_function;
    }

    void SimulatedAnnealingTSPSolver::setMarkovChainLengthScale(float markovScale)
    {
        this->markov_chain_lenght_scale = markovScale;
    }

    void SimulatedAnnealingTSPSolver::setIterationsLimit(size_t stops_count, size_t limit)
    {
        this->limits[stops_count] = limit;
    }
}
