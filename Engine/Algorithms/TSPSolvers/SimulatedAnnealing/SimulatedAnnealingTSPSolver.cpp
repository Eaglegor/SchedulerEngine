#include "SimulatedAnnealingTSPSolver.h"
#include <random>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include "SolutionGenerator.h"
#include "TemperatureScheduler.h"

namespace Scheduler
{
    SimulatedAnnealingTSPSolver::SimulatedAnnealingTSPSolver():
        schedule_cost_function(nullptr),
        temperature_scheduler(nullptr),
        markov_chain_length_scale(1.f),
        allowed_mutations({SolutionGenerator::MutationType::BlockInsert,
                          SolutionGenerator::MutationType::BlockReverse,
                          SolutionGenerator::MutationType::VertexInsert,
                          SolutionGenerator::MutationType::VertexSwap})
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
        if (allowed_mutations.empty()) return;

        if (run->getWorkStops().size() <= 1) {
            return;
        }

        SolutionGenerator solution_generator(run);
        for (const auto mutation : allowed_mutations) {
            solution_generator.enableMutation(mutation);
        }

        temperature_scheduler->initialize(run, schedule_cost_function);
        Cost best_cost = schedule_cost_function->calculateCost(run->getSchedule());

        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_real_distribution<float> float_distribution(0.f, 1.f);

        const size_t M = markovChainLength(run->getWorkStops().size());
        while (!temperature_scheduler->isFinish()) {
            for (size_t m = 0; m < M; ++m) {
                solution_generator.neighbour();
                const Cost cost = schedule_cost_function->calculateCost(run->getSchedule());
                if (cost < best_cost) {
                    best_cost = cost;
                    solution_generator.store();
                } else {
                    const float random_value = float_distribution(random_engine);
                    if (acceptance(cost - best_cost, random_value)) {
                        temperature_scheduler->adapt(cost - best_cost, random_value);
                        best_cost = cost;
                        solution_generator.store();
                    } else {
                        solution_generator.discard();
                    }
                }
            }
            temperature_scheduler->changeTemperature();
        }
    }

    bool SimulatedAnnealingTSPSolver::acceptance(Cost delta, float random) const
    {
        const float normalized_value = 1 / (1 + std::exp(delta.getValue() / temperature_scheduler->getTemperature()));
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
        this->markov_chain_length_scale = markovScale;
    }

    void SimulatedAnnealingTSPSolver::setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations)
    {
        this->allowed_mutations.clear();
        this->allowed_mutations.insert(mutations.begin(), mutations.end());
    }

    size_t SimulatedAnnealingTSPSolver::markovChainLength(size_t stopsCount) const
    {
        const size_t M = std::nearbyint(stopsCount * std::log(stopsCount) * markov_chain_length_scale);
        return std::max(1ul, M);
    }

}
