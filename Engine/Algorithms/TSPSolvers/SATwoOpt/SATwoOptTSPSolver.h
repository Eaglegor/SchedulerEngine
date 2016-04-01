#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include "TemperatureScheduler.h"
#include <SATwoOptTSPSolver_export.h>

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT SATwoOptTSPSolver : public TSPSolver
	{
	public:
        SATwoOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setTemperatureScheduler(TemperatureScheduler* temperature_scheduler);

        static constexpr const char* staticGetName() { return "SATwoOpt"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        bool isAccepting(Cost delta, float random) const;

		ScheduleCostFunction* schedule_cost_function;
        TemperatureScheduler* temperature_scheduler;
	};
}
