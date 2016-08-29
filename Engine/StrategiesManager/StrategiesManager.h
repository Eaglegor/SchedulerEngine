#pragma once

#include <Engine/Utils/Factory.h>
#include <unordered_set>
#include "VRPSolver.h"
#include "TSPSolver.h"

#include "CostFunctions/RunCostFunction.h"
#include "CostFunctions/ScheduleCostFunction.h"
#include "CostFunctions/SceneCostFunction.h"

#include <StrategiesManager_export.h>

namespace Scheduler
{
	class Strategy;

	class STRATEGIESMANAGER_EXPORT StrategiesManager
	{
	public:
		StrategiesManager();
		~StrategiesManager();

		Strategy* createStrategy();
		void destroyStrategy(Strategy* strategy);

	private:
		Factory<VRPSolver> vrp_solvers_factory;
		Factory<TSPSolver> tsp_solvers_factory;

		Factory<RunCostFunction> run_cost_functions_factory;
		Factory<ScheduleCostFunction> schedule_cost_functions_factory;
		Factory<SceneCostFunction> scene_cost_functions_factory;

		Factory<Strategy> strategies_factory;

		std::unordered_set<Strategy*> strategies;
	};
}