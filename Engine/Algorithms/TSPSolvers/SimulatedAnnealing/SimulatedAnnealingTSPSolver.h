#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>
#include <Engine/Utils/Optional.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include "InstanceBasedSolutionGenerator.h"

namespace Scheduler
{
    class ScheduleCostFunction;
    class TemperatureScheduler;
    class Cost;
    class ScheduleVariant;

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT SimulatedAnnealingTSPSolver : public TSPSolver
	{
	public:
        enum class MultithreadingType
        {
            Auto,
            Independent,
            Iterative
        };

        SimulatedAnnealingTSPSolver();

		virtual void optimize(Schedule& schedule) const override;
        virtual void optimize(Run& run) const override;

		void setScheduleCostFunction(const ScheduleCostFunction& cost_function);
        void setTemperatureScheduler(const TemperatureScheduler& temperature_scheduler_template);
        void setMarkovChainLengthScale(float markov_scale);
        void setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations);
        void setThreadsNumber(std::size_t threads_number);
        void setMultithreadingType(MultithreadingType multithreading_type);
        void setPopulationSize(std::size_t population_size);

        static constexpr const char* staticGetName() { return "SimulatedAnnealing"; }
		virtual const char* getName() const override { return staticGetName(); };

    private:
        typedef std::vector<ScheduleVariant> VectorSchedules;
        typedef std::vector<ReferenceWrapper<Run>> VectorRuns;
        typedef std::vector<Cost> VectorCosts;
        typedef std::vector<std::unique_ptr<InstanceBasedSolutionGenerator>> VectorGenerators;
        typedef std::vector<std::unique_ptr<TemperatureScheduler>> VectorTemperatureSchedulers;

        bool acceptance(Cost delta, float random, float temperature) const;
        std::size_t markovChainLength (std::size_t stops_count) const;
        void initialize(Run& run,
                        VectorSchedules& schedules,
                        VectorRuns& runs,
                        VectorCosts& costs,
                        VectorGenerators& generators,
                        VectorTemperatureSchedulers& temperature_schedulers) const;
        void optimizeAuto(Run& run) const;
        void optimizeIndependent(Run& run) const;
        void optimizeIterative(Run& run) const;
        void step(const Run& run,
                  std::mt19937_64& random_engine,
                  TemperatureScheduler& temp_sched,
                  InstanceBasedSolutionGenerator& solution_generator,
                  Cost& best_cost) const;

		Optional<const ScheduleCostFunction&> schedule_cost_function;
        Optional<const TemperatureScheduler&> temperature_scheduler_template;
        std::set<SolutionGenerator::MutationType> allowed_mutations;
        float markov_chain_length_scale;
        std::size_t population_size;
        std::size_t threads_number;
        MultithreadingType multithreading_type;
        Logger& logger;
    };
}
