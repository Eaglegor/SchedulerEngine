#pragma once

#include <Engine/AlgorithmsManager/TSPSolver.h>
#include <vector>
#include <IterativeTSPSolver_export.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/Utils/Optional.h>
#include <Engine/Utils/ReferenceWrapper.h>

namespace Scheduler
{
    class ITERATIVETSPSOLVER_EXPORT IterativeTSPSolver : public TSPSolver
	{
	public:
		IterativeTSPSolver(const TSPSolver& backend_solver, const ScheduleCostFunction& schedule_cost_function);

		virtual void optimize(Schedule& schedule) const override;
		virtual void optimize(Run& run) const override;

        static constexpr const char* staticGetName() { return "Iterative"; }
		virtual const char* getName() const override { return staticGetName(); };

		void setIterationsCount(std::size_t iterations_count);
		
	private:
        const TSPSolver& backend_solver;
		const ScheduleCostFunction& schedule_cost_function;
		std::size_t iterations_count;
		Logger& logger;
	};
}
