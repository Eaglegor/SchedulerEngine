#pragma once

#include <Engine/SceneManager/Factory.h>
#include <unordered_set>
#include "VRPSolver.h"
#include "TSPSolver.h"
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Strategy;

	class SCHEDULERENGINE_EXPORT StrategiesManager
	{
	public:
		~StrategiesManager();

		Strategy* createStrategy();
		void destroyStrategy(Strategy* strategy);

	private:
		Factory<VRPSolver> vrp_solvers_factory;
		Factory<TSPSolver> tsp_solvers_factory;

		Factory<Strategy> strategies_factory;

		std::unordered_set<Strategy*> strategies;
	};
}