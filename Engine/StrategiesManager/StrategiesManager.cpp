#include "StrategiesManager.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include <assert.h>
#include "Strategy.h"

namespace Scheduler
{
	StrategiesManager::StrategiesManager(MemoryManager* memory_manager):
		vrp_solvers_factory(memory_manager),
		tsp_solvers_factory(memory_manager),
		run_cost_functions_factory(memory_manager),
		schedule_cost_functions_factory(memory_manager),
		scene_cost_functions_factory(memory_manager),
		strategies_factory(memory_manager)
	{
	}

	StrategiesManager::~StrategiesManager()
	{
		for(Strategy* strategy : strategies)
		{
			strategies_factory.destroyObject(strategy);
		}
	}

	Strategy* StrategiesManager::createStrategy()
	{
		Strategy* strategy = strategies_factory.createObject<Strategy>();
		
		if (!strategy) return nullptr;

		strategy->setTSPSolversFactory(&tsp_solvers_factory);
		strategy->setVRPSolversFactory(&vrp_solvers_factory);

		strategy->setRunCostFunctionsFactory(&run_cost_functions_factory);
		strategy->setScheduleCostFunctionsFactory(&schedule_cost_functions_factory);
		strategy->setSceneCostFunctionsFactory(&scene_cost_functions_factory);

		strategies.emplace(strategy);
		
		return strategy;
	}

	void StrategiesManager::destroyStrategy(Strategy* strategy)
	{
		assert(std::contains_key(strategies, strategy));

		strategies_factory.destroyObject(strategy);

		strategies.erase(strategy);
	}
}
