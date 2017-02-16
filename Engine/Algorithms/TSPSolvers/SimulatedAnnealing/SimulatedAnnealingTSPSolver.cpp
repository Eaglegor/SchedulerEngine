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
        logger(LoggingManager::getLogger("SimulatedAnnealingTSPSolver")),
        markov_chain_length_scale(1.f),
        population_size(2),
        threads_number(1)
    {
    }

    void SimulatedAnnealingTSPSolver::optimize(Schedule& schedule) const
    {
        TRACEABLE_SECTION(__optimize__, "optimize(Schedule&)", logger);

        if (!schedule_cost_function) {
            LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
            return;
        }

        if (!temperature_scheduler_template) {
            LOG_WARNING(logger, "Temperature scheduler is not set. Can't solve TSP.");
            return;
        }

        if (allowed_mutations.empty()) {
            LOG_WARNING(logger, "Allowed mutations is not set. Can't solve TSP.");
            return;
        }

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

    void SimulatedAnnealingTSPSolver::setPopulationSize(size_t population_size)
    {
        this->population_size = std::max(static_cast<std::size_t>(2), population_size);
    }

    void SimulatedAnnealingTSPSolver::setThreadsNumber(std::size_t threads_number)
    {
        this->threads_number = threads_number;
    }

    void SimulatedAnnealingTSPSolver::optimize(Run& run) const
    {
        TRACEABLE_SECTION(__optimize__, "optimize(Run&)", logger);

        if (!schedule_cost_function) {
            LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
            return;
        }

        if (!temperature_scheduler_template) {
            LOG_WARNING(logger, "Temperature scheduler is not set. Can't solve TSP.");
            return;
        }

        if (allowed_mutations.empty()) {
            LOG_WARNING(logger, "Allowed mutations is not set. Can't solve TSP.");
            return;
        }

        if (run.getWorkStops().size() <= 1) {
            LOG_WARNING(logger, "Too small run size. Can't solve TSP.");
            return;
        }

        LOG_DEBUG(logger, "optimize run for {} stops at {} agents", run.getWorkStops().size(), population_size);

        LOG_TRACE(logger, "initializing");

        auto temperature_scheduler = std::unique_ptr<TemperatureScheduler>(temperature_scheduler_template->clone());
        temperature_scheduler->initialize(run, schedule_cost_function.get());

        auto run_iter = run.getSchedule().findRun(run);
        const std::size_t run_idx = std::distance<Schedule::ConstRunIterator>(run.getSchedule().getRuns().begin(), run_iter);
		
        std::vector<ScheduleVariant> schedules;
        std::vector<ReferenceWrapper<Run>> runs;
        std::vector<Cost> costs;
        std::vector<std::shared_ptr<InstanceBasedSolutionGenerator>> generators;
        const std::size_t N = population_size;
        for (std::size_t idx = 0; idx < N; ++idx) {
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

        std::size_t number_of_iterations = 0;
        std::size_t total_acceptance_number = 0;
        std::size_t total_mutations_number = 0;
        std::size_t total_best_costs_number = 0;

        const std::size_t S = markovChainLength(run.getWorkStops().size());
        const std::size_t T = std::min(threads_number, population_size);(void)T;//used for openmp
        TRACEABLE_SECTION(__main_loop__,  "main loop", logger);
        while (!temperature_scheduler->isFinish()) {
            TRACEABLE_SECTION(__outer_iteration__, "outer iteration", logger);
            LOG_DEBUG(logger, "set temperature to {} on iteration {}", temperature_scheduler->getTemperature(), number_of_iterations);
            std::size_t acceptance_number = 0;
            std::size_t mutations_number = 0;
            std::size_t best_costs_number = 0;
            #pragma omp parallel for num_threads(T) if (T > 1)
            for (std::size_t i = 0; i < N; ++i) {
                auto solution_generator = generators[i];
                solution_generator->setPopulations(populations, i);
                auto run_ref = runs[i];
                Cost &best_cost = costs[i];
                for (size_t s = 0; s < S; ++s) {
                    solution_generator->neighbour();
                    const Cost cost = solution_generator->hasPermutation() ? schedule_cost_function->calculateCost(run_ref.get().getSchedule()) : best_cost;
                    mutations_number += (solution_generator->hasPermutation() ? 1 : 0);
                    if (cost < best_cost) {
                        best_cost = cost;
                        solution_generator->store();
                        ++best_costs_number;
                    } else {
                        const float random_value = float_distribution(random_engine);
                        const Cost delta = cost - best_cost;
                        if (acceptance(delta, random_value, temperature_scheduler->getTemperature())) {
                            #pragma omp critical
                            {
                                temperature_scheduler->adapt(delta, random_value);
                                ++acceptance_number;
                            }
                            best_cost = cost;
                            solution_generator->store();
                        } else {
                            solution_generator->discard();
                        }
                    }
                }
                InstanceBasedSolutionGenerator::VectorSizeT & vector_of_idx = populations_write[i];
                vector_of_idx.clear();
                for (auto & workStop : run_ref.get().getWorkStops()) {
                    vector_of_idx.push_back(workStop.getOperation().getId());
                }
            }
            LOG_DEBUG(logger, "iteration number {} acceptances is {} mutations is {} best costs {}", number_of_iterations, acceptance_number, mutations_number, best_costs_number);
            temperature_scheduler->changeTemperature();
            std::swap(populations, populations_write);
            ++number_of_iterations;
            total_acceptance_number += acceptance_number;
            total_mutations_number += mutations_number;
            total_best_costs_number += best_costs_number;
        }

        LOG_DEBUG(logger, "final temperature is {} on iteration {}", temperature_scheduler->getTemperature(), number_of_iterations);
        LOG_DEBUG(logger, "total number of iterations {}", number_of_iterations * M * S);
        LOG_DEBUG(logger, "total number of acceptance {}", total_acceptance_number);
        LOG_DEBUG(logger, "total number of mutations {}", total_mutations_number);
        LOG_DEBUG(logger, "total number of best cost {}", total_best_costs_number);

        auto best_cost_iter = std::min_element(costs.begin(), costs.end());
		ScheduleVariant& best_variant = schedules.at(std::distance(costs.begin(), best_cost_iter));
		best_variant.apply();
    }
}
