#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include "TemperatureScheduler.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    enum class SimulatedAnnealingType
    {
        Default,
        Greedy
    };

    class SATWOOPTTSPSOLVER_EXPORT SATwoOptTSPSolver : public TSPSolver
	{
	public:
        SATwoOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setTemperatureScheduler(TemperatureScheduler* temperature_scheduler);
        void setType(SimulatedAnnealingType type);

        static constexpr const char* staticGetName() { return "Simulated Annealing"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        virtual void default_optimize(Run* schedule) const;
        virtual void greedy_optimize(Run* schedule) const;
        bool acceptance(Cost delta, float random) const;

		ScheduleCostFunction* schedule_cost_function;
        TemperatureScheduler* temperature_scheduler;
        SimulatedAnnealingType type;
	};
}
