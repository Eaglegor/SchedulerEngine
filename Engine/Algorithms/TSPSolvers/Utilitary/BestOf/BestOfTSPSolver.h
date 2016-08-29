#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <vector>
#include <BestOfTSPSolver_export.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/LoggingManager/LoggingManager.h>

namespace Scheduler
{
    class BESTOFTSPSOLVER_EXPORT BestOfTSPSolver : public TSPSolver
	{
	public:
		BestOfTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

        void addTSPSolver(TSPSolver* aTSPSolver);
        void setScheduleCostFunction(ScheduleCostFunction* cost_function);
		
		void setConcurrencyEnabled(bool value);

        static constexpr const char* staticGetName() { return "BestOf"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        virtual void concurrentOptimize(Schedule* schedule) const;
        virtual void concurrentOptimize(Run* schedule) const;
        virtual void sequentialOptimize(Schedule* schedule) const;
        virtual void sequentialOptimize(Run* schedule) const;

        std::vector<TSPSolver*> tsp_solvers;
        ScheduleCostFunction* schedule_cost_function;
		bool concurrency_enabled;
		Logger* logger;
	};
}
