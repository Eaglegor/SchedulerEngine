#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <cstddef>
#include <SimulatedAnnealingTSPSolver_export.h>
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

		virtual void optimize(Schedule* schedule) const override;
        virtual void optimize(Run* run) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setTemperatureScheduler(TemperatureScheduler* temperature_scheduler);
        void setMarkovChainLengthScale(float markovScale);
        void setMutations(std::initializer_list<SolutionGenerator::MutationType> mutations);

        static constexpr const char* staticGetName() { return "Simulated Annealing"; }
		virtual const char* getName() const override { return staticGetName(); };

    protected:
        bool acceptance(Cost delta, float random) const;
        size_t markovChainLength (size_t stopsCount) const;

		ScheduleCostFunction* schedule_cost_function;
        TemperatureScheduler* temperature_scheduler;
        float markov_chain_length_scale;

        std::set<SolutionGenerator::MutationType> allowed_mutations;
	};

    class SIMULATEDANNEALINGTSPSOLVER_EXPORT MultiAgentSimulatedAnnealingTSPSolver : public SimulatedAnnealingTSPSolver
    {
    public:
        MultiAgentSimulatedAnnealingTSPSolver();
        virtual void optimize(Run* run) const override;

        void setThreadsNumber(size_t threadsNumber);
        void setPopulationScale(size_t populationScale);

        static constexpr const char* staticGetName() { return "Multi Agent Simulated Annealing"; }
        virtual const char* getName() const override { return staticGetName(); };
    private:

        size_t population_scale;
        size_t threads_number;
    };
}
