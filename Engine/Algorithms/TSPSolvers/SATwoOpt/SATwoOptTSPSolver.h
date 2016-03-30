#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <SATwoOptTSPSolver_export.h>
#include "AcceptanceFunction.h"
#include "TemperatureFunction.h"

namespace Scheduler
{
    class SATWOOPTTSPSOLVER_EXPORT SATwoOptTSPSolver : public TSPSolver
	{
	public:
        SATwoOptTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

		void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setAcceptanceFunction(AcceptanceFunction* acceptance_function);
        void setTemperatureFunction(TemperatureFunction* temperature_function);

        static constexpr const char* staticGetName() { return "SATwoOpt"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
		ScheduleCostFunction* schedule_cost_function;
        AcceptanceFunction* acceptance_function;
        TemperatureFunction* temperature_function;
	};
}
