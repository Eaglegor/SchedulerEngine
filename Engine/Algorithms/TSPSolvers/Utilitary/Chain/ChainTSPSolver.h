#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <ChainTSPSolver_export.h>
#include <vector>
#include <Engine/Utils/ReferenceWrapper.h>

namespace Scheduler
{
    class CHAINTSPSOLVER_EXPORT ChainTSPSolver : public TSPSolver
	{
	public:
		ChainTSPSolver();

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& schedule) const override;

        void addTSPSolver(const TSPSolver& aTSPSolver);

        static constexpr const char* staticGetName() { return "Chain"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        std::vector<ReferenceWrapper<const TSPSolver>> tsp_solvers;
	};
}
