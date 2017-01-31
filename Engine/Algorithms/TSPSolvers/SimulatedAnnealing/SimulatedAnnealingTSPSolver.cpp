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
        //const float normalized_value = 1 / (1 + std::exp(delta.getValue() / temperature_scheduler->getTemperature()));
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

    void SimulatedAnnealingTSPSolver::setMarkovChainLengthScale(float markovScale)
    {
        this->markov_chain_length_scale = markovScale;
    }

    void SimulatedAnnealingTSPSolver::setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations)
    {
        this->allowed_mutations.clear();
        this->allowed_mutations.insert(mutations.begin(), mutations.end());
    }

    std::size_t SimulatedAnnealingTSPSolver::markovChainLength(std::size_t stopsCount) const
    {
        //const size_t M = std::nearbyint(stopsCount * std::log(stopsCount) * markov_chain_length_scale);
        const size_t M = std::nearbyint(stopsCount * markov_chain_length_scale);
        return std::max(std::size_t(1), M);
    }

    void SimulatedAnnealingTSPSolver::setPopulationScale(size_t populationScale)
    {
        population_scale = std::max(static_cast<std::size_t>(2), populationScale);
    }

    void SimulatedAnnealingTSPSolver::setThreadsNumber(std::size_t threadsNumber)
    {
        this->threads_number = threadsNumber;
    }

    void SimulatedAnnealingTSPSolver::addTSPSolver(const Scheduler::TSPSolver &aTSPSolver)
    {
        tsp_solvers.emplace_back(aTSPSolver);
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

        auto runIter = run.getSchedule().findRun(run);
        const std::size_t runIdx = std::distance<Schedule::ConstRunIterator>(run.getSchedule().getRuns().begin(), runIter);
		
        std::vector<ScheduleVariant> schedules;
        std::vector<ReferenceWrapper<Run>> runs;
        std::vector<Cost> costs;
        std::vector<std::shared_ptr<InstanceBasedSolutionGenerator>> generators;
        const std::size_t population_size = threads_number * population_scale;
        for (std::size_t idx = 0; idx < population_size; ++idx) {
            schedules.emplace_back(run.getSchedule());
            ScheduleVariant& temporarySchedule = schedules.back();

            Run& temporaryRun = temporarySchedule.getSchedule()->getRuns().at(runIdx);
            runs.push_back(temporaryRun);

            const TSPSolver& tsp_solver = tsp_solvers.at(idx % tsp_solvers.size());
            tsp_solver.optimize(temporaryRun);
            
            auto solution_generator = std::make_shared<InstanceBasedSolutionGenerator>(temporaryRun);
            for (const auto mutation : allowed_mutations) {
                solution_generator->enableMutation(mutation);
            }
            generators.push_back(solution_generator);

            const Cost initialCost = schedule_cost_function->calculateCost(temporaryRun.getSchedule());
            costs.push_back(initialCost);
        }

        const std::size_t T = threads_number;
        for (std::size_t g = 0; g < generators.size() / T; ++g) {
            auto concurrent_populations = runs;
            concurrent_populations.erase(std::next(concurrent_populations.begin(), g * T), std::next(concurrent_populations.begin(), g * T + T));
            for (size_t i = 0; i < T; ++i) {
                generators.at(g * T + i)->setPopulations(concurrent_populations);
            }
        }

        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_real_distribution<float> float_distribution(0.f, 1.f);

        const std::size_t M = population_size;
        const std::size_t S = markovChainLength(run.getWorkStops().size());
        while (!temperature_scheduler->isFinish()) {
            for (std::size_t g = 0; g < M / T; ++g) {
                bool stop = false;
                #pragma omp parallel for num_threads(threads_number) if (threads_number > 1)
                for (int m = 0; m < T; ++m) {
                    auto solution_generator = generators[g * T + m];
                    auto concurrent_populations = runs;
                    concurrent_populations.erase(std::next(concurrent_populations.begin(), g * T), std::next(concurrent_populations.begin(), g * T + T));
                    solution_generator->setPopulations(concurrent_populations);
                    auto runRef = runs[g * T + m];
                    Cost &best_cost = costs[g * T + m];
                    for (size_t s = 0; s < S && !stop; ++s) {
                        solution_generator->neighbour();
                        const Cost cost = solution_generator->hasPermutation() ? schedule_cost_function->calculateCost(runRef.get().getSchedule()) : best_cost;
                        if (cost < best_cost) {
                            best_cost = cost;
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
                    stop = true;
                }
            }
            temperature_scheduler->changeTemperature();
        }

        auto best_cost_iter = std::min_element(costs.begin(), costs.end());
		ScheduleVariant& best_variant = schedules.at(std::distance(costs.begin(), best_cost_iter));
		best_variant.apply();
    }
}
