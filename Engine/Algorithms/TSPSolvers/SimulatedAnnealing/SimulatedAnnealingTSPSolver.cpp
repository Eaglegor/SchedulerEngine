#include "SimulatedAnnealingTSPSolver.h"
#include <random>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Utils/SceneCloner.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/ScheduleVariant.h>
#include "SolutionGenerator.h"
#include "TemperatureScheduler.h"

namespace Scheduler
{
    SimulatedAnnealingTSPSolver::SimulatedAnnealingTSPSolver():
        allowed_mutations({SolutionGenerator::MutationType::BlockInsert,
                          SolutionGenerator::MutationType::BlockReverse,
                          SolutionGenerator::MutationType::VertexInsert,
                          SolutionGenerator::MutationType::VertexSwap}),
        markov_chain_length_scale(1.f),
        population_scale(2),
        threads_number(1)
    {
    }

    void SimulatedAnnealingTSPSolver::optimize(Schedule& schedule) const
    {
        if (!schedule_cost_function) return;
        if (!temperature_scheduler_template) return;

        for(Run& run : schedule.getRuns())
        {
            optimize(run);
        }
    }

    bool SimulatedAnnealingTSPSolver::acceptance(Cost delta, float random, float temperature) const
    {
        const float normalized_value = std::exp(-delta.getValue() / temperature);
        return random <= normalized_value;
    }

    void SimulatedAnnealingTSPSolver::setScheduleCostFunction(const ScheduleCostFunction& cost_function)
    {
        this->schedule_cost_function = cost_function;
    }

    void SimulatedAnnealingTSPSolver::setTemperatureScheduler(TemperatureScheduler &temperature_function)
    {
        this->temperature_scheduler_template = temperature_function;
    }

    void SimulatedAnnealingTSPSolver::setMarkovChainLengthScale(float markov_scale)
    {
        this->markov_chain_length_scale = markov_scale;
    }

    void SimulatedAnnealingTSPSolver::setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations)
    {
        this->allowed_mutations.clear();
        this->allowed_mutations.insert(mutations.begin(), mutations.end());
    }

    std::size_t SimulatedAnnealingTSPSolver::markovChainLength(std::size_t stops_count) const
    {
        const size_t M = std::nearbyint(stops_count * markov_chain_length_scale);
        return std::max(std::size_t(1), M);
    }

    void SimulatedAnnealingTSPSolver::setPopulationScale(size_t population_scale)
    {
        this->population_scale = std::max(static_cast<std::size_t>(1), population_scale);
    }

    void SimulatedAnnealingTSPSolver::setThreadsNumber(std::size_t threads_number)
    {
        this->threads_number = threads_number;
    }

    void SimulatedAnnealingTSPSolver::optimize(Run& run) const
    {
        if (!schedule_cost_function) return;
        if (!temperature_scheduler_template) return;
        if (allowed_mutations.empty()) return;

        if (run.getWorkStops().size() <= 1) {
            return;
        }

        auto temperature_scheduler = std::unique_ptr<TemperatureScheduler>(temperature_scheduler_template->clone());
        temperature_scheduler->initialize(run, schedule_cost_function.get());

        auto run_iter = run.getSchedule().findRun(run);
        const std::size_t run_idx = std::distance<Schedule::ConstRunIterator>(run.getSchedule().getRuns().begin(), run_iter);
		
        std::vector<ScheduleVariant> schedules;
        std::vector<ReferenceWrapper<Run>> runs;
        std::vector<Cost> costs;
        std::vector<std::shared_ptr<InstanceBasedSolutionGenerator>> generators;
        const std::size_t population_size = threads_number * population_scale;
        for (std::size_t idx = 0; idx < population_size; ++idx) {
            schedules.emplace_back(run.getSchedule());
            ScheduleVariant& temporary_schedule = schedules.back();

            Run& temporary_run = temporary_schedule.getSchedule()->getRuns().at(run_idx);
            runs.push_back(temporary_run);
            
            auto solution_generator = std::make_shared<InstanceBasedSolutionGenerator>(temporary_run);
            for (const auto mutation : allowed_mutations) {
                solution_generator->enableMutation(mutation);
            }
            solution_generator->shuffle();
            solution_generator->store();
            generators.push_back(solution_generator);

            const Cost initial_cost = schedule_cost_function->calculateCost(temporary_run.getSchedule());
            costs.push_back(initial_cost);
        }

        const std::size_t T = threads_number;

        InstanceBasedSolutionGenerator::PopulationsT populations;
        InstanceBasedSolutionGenerator::PopulationsT populations_write;
        for (Run& run_ref : runs) {
            InstanceBasedSolutionGenerator::VectorSizeT vector_of_idx;
            for (auto & workStop : run_ref.getWorkStops()) {
                vector_of_idx.push_back(workStop.getOperation().getId());
            }
            populations.emplace_back(vector_of_idx);
        }
        populations_write.resize(populations.size());


        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_real_distribution<float> float_distribution(0.f, 1.f);

        Cost total_best_cost = costs.front();
        const std::size_t M = population_size;
        const std::size_t S = markovChainLength(run.getWorkStops().size());
        while (!temperature_scheduler->isFinish()) {
            for (std::size_t g = 0; g < M / T; ++g) {
                #pragma omp parallel for num_threads(threads_number) if (threads_number > 1)
                for (int m = 0; m < T; ++m) {
                    auto solution_generator = generators[g * T + m];
                    solution_generator->setPopulations(populations, g * T + m);
                    auto run_ref = runs[g * T + m];
                    Cost &best_cost = costs[g * T + m];
                    for (size_t s = 0; s < S; ++s) {
                        solution_generator->neighbour();
                        const Cost cost = solution_generator->hasPermutation() ? schedule_cost_function->calculateCost(run_ref.get().getSchedule()) : best_cost;
                        if (cost < best_cost) {
                            best_cost = cost;
                            if (best_cost < total_best_cost) {
                                total_best_cost = best_cost;
                            }
                            solution_generator->store();
                        } else {
                            const float random_value = float_distribution(random_engine);
                            const Cost delta = cost - best_cost;
                            if (acceptance(delta, random_value, temperature_scheduler->getTemperature())) {
                                #pragma omp critical
                                {
                                    temperature_scheduler->adapt(delta, random_value);
                                }
                                best_cost = cost;
                                solution_generator->store();
                            } else {
                                solution_generator->discard();
                            }
                        }
                    }
                    InstanceBasedSolutionGenerator::VectorSizeT & vector_of_idx = populations_write[g * T + m];
                    vector_of_idx.clear();
                    for (auto & workStop : run_ref.get().getWorkStops()) {
                        vector_of_idx.push_back(workStop.getOperation().getId());
                    }
                }
            }
            temperature_scheduler->changeTemperature();
            std::swap(populations, populations_write);
        }

        auto best_cost_iter = std::min_element(costs.begin(), costs.end());
		ScheduleVariant& best_variant = schedules.at(std::distance(costs.begin(), best_cost_iter));
		best_variant.apply();
    }
}
