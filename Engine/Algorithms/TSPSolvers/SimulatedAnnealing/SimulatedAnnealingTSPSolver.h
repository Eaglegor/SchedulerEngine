#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>
#include <Engine/Utils/Optional.h>
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
        void setTemperatureScheduler(TemperatureScheduler& temperature_scheduler);
        void setMarkovChainLengthScale(float markovScale);
        void setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations);

        static constexpr const char* staticGetName() { return "SimulatedAnnealing"; }
		virtual const char* getName() const override { return staticGetName(); };

    protected:
        bool acceptance(Cost delta, float random) const;
        std::size_t markovChainLength (std::size_t stopsCount) const;

		Optional<const ScheduleCostFunction&> schedule_cost_function;
        Optional<TemperatureScheduler&> temperature_scheduler;
        float markov_chain_length_scale;

        std::set<SolutionGenerator::MutationType> allowed_mutations;
	};

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT MultiAgentSimulatedAnnealingTSPSolver : public SimulatedAnnealingTSPSolver
    {
    public:
        MultiAgentSimulatedAnnealingTSPSolver();
        virtual void optimize(Run& run) const override;

        void setThreadsNumber(std::size_t threadsNumber);
        void setPopulationScale(std::size_t populationScale);

        static constexpr const char* staticGetName() { return "MultiAgentSimulatedAnnealing"; }
        virtual const char* getName() const override { return staticGetName(); };
		
    private:
        std::size_t population_scale;
        std::size_t threads_number;
    };
}
