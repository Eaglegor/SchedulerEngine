#pragma once

#include <Engine/StrategiesManager/TSPSolver.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/Engine/Services/RoutingService.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <Engine/Utils/Factory.h>
#include <memory>
#include <SuIntTSPSolver_export.h>
#include "EdgeSuggestorType.h"
#include "EdgeIntroducerType.h"
#include "EdgeSuggestor.h"
#include "EdgeIntroducer.h"

namespace Scheduler
{
    class SUINTTSPSOLVER_EXPORT SuIntTSPSolver : public TSPSolver
	{
	public:
		SuIntTSPSolver();

		virtual void optimize(Schedule* schedule) const override;
		virtual void optimize(Run* run) const override;

        void setRoutingService(RoutingService* routing_service);
		void setCostFunction(ScheduleCostFunction* cost_function);

        static constexpr const char* staticGetName() { return "SuInt"; }
		virtual const char* getName() const override { return staticGetName(); };

		void setEdgeSuggestor(const EdgeSuggestorType& type);
		void addEdgeIntroducer(const EdgeIntroducerType& type);

	private:
        RoutingService* routing_service;
		ScheduleCostFunction* cost_function;
		Logger* logger;

		EdgeSuggestorType edge_suggestor_type;
		std::vector<EdgeIntroducerType> edge_introducers_types;
	};
}
