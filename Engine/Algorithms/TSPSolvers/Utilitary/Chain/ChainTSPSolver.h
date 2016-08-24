#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <ChainTSPSolver_export.h>
#include <vector>

namespace Scheduler
{
    class CHAINTSPSOLVER_EXPORT ChainTSPSolver : public TSPSolver
	{
	public:
		ChainTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* schedule) const override;

        void addTSPSolver(TSPSolver* aTSPSolver);

        static constexpr const char* staticGetName() { return "Chain"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        std::vector<TSPSolver*> tsp_solvers;
	};
}
