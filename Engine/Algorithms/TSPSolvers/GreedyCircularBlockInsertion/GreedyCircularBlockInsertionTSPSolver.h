#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/Engine/Services/RoutingService.h>
#include <GreedyCircularBlockInsertionTSPSolver_export.h>

namespace Scheduler
{
    class GREEDYCIRCULARBLOCKINSERTIONTSPSOLVER_EXPORT GreedyCircularBlockInsertionTSPSolver : public TSPSolver
	{
	public:
		GreedyCircularBlockInsertionTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* run) const override;

        void setRoutingService(RoutingService* routing_service);
		void setCostFunction(ScheduleCostFunction* cost_function);

        static constexpr const char* staticGetName() { return "GreedyCircularBlockInsertion"; }
		virtual const char* getName() const override { return staticGetName(); };

	private:
        RoutingService* routing_service;
		ScheduleCostFunction* cost_function;
	};
}
