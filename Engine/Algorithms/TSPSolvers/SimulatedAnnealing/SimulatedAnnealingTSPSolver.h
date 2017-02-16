#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>
#include <Engine/Utils/Optional.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include "SolutionGenerator.h"

namespace Scheduler
{
    class ScheduleCostFunction;
    class TemperatureScheduler;
    class Cost;

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT SimulatedAnnealingTSPSolver : public TSPSolver
	{
	public:
        SimulatedAnnealingTSPSolver();

		virtual void optimize(Schedule& schedule) const override;
        virtual void optimize(Run& run) const override;

		void setScheduleCostFunction(const ScheduleCostFunction& cost_function);
        void setTemperatureScheduler(TemperatureScheduler& temperature_scheduler_template);
        void setMarkovChainLengthScale(float markovScale);
        void setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations);
        void setThreadsNumber(std::size_t threads_number);
        void setPopulationSize(std::size_t population_size);

        static constexpr const char* staticGetName() { return "SimulatedAnnealing"; }
		virtual const char* getName() const override { return staticGetName(); };

    private:
        bool acceptance(Cost delta, float random, float temperature) const;
        std::size_t markovChainLength (std::size_t stops_count) const;

		Optional<const ScheduleCostFunction&> schedule_cost_function;
        Optional<TemperatureScheduler&> temperature_scheduler_template;
        std::set<SolutionGenerator::MutationType> allowed_mutations;

        Logger& logger;

        float markov_chain_length_scale;
        std::size_t population_size;
        std::size_t threads_number;
    };
}
