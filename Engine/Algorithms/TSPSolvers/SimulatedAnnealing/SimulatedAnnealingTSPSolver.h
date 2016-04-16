#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <cstddef>
#include <map>
#include <SimulatedAnnealingTSPSolver_export.h>

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
        void seed(unsigned long long value);
        void setUseAdaptiveOps(bool useAdaptiveOps);
        void setMarkovScale(float markovScale);
        void setIterationsLimit(size_t stops_count, size_t limit);

        static constexpr const char* staticGetName() { return "Simulated Annealing"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        bool acceptance(Cost delta, float random) const;

        std::map<size_t, size_t> limits;

		ScheduleCostFunction* schedule_cost_function;
        TemperatureScheduler* temperature_scheduler;

        unsigned long long seed_value;

        bool use_adaptive_ops;
        float markov_scale;
	};
}
