#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <vector>
#include <TheBestTSPSolver_export.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{
    class THEBESTTSPSOLVER_EXPORT TheBestTSPSolver : public TSPSolver
	{
	public:
		TheBestTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

        void addTSPSolver(TSPSolver* aTSPSolver);
        void setScheduleCostFunction(ScheduleCostFunction* cost_function);
        void setNumberOfThreads(unsigned number_of_threads);

        static constexpr const char* staticGetName() { return "TheBest"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        virtual void concurrentOptimize(Schedule* schedule) const;
        virtual void concurrentOptimize(Run* schedule) const;
        virtual void boostOptimize(Schedule* schedule) const;
        virtual void boostOptimize(Run* schedule) const;
        virtual void sequentialOptimize(Schedule* schedule) const;
        virtual void sequentialOptimize(Run* schedule) const;

        std::vector<TSPSolver*> tsp_solvers;
        ScheduleCostFunction* schedule_cost_function;
        unsigned number_of_threads;
	};
}
