#include "StrategiesManager.h"
#include <Engine/Utils/Collections/Algorithms.h>
#include <assert.h>
#include "Strategy.h"
#include <Engine/MemoryManager/ObjectSizes.h>

namespace Scheduler
{
	StrategiesManager::StrategiesManager():
		vrp_solvers_factory( Pool::MEDIUM_OBJECT, 10),
		tsp_solvers_factory(Pool::MEDIUM_OBJECT, 10),
		run_cost_functions_factory(Pool::SMALL_OBJECT, 5),
		schedule_cost_functions_factory(Pool::SMALL_OBJECT, 5),
		scene_cost_functions_factory(Pool::SMALL_OBJECT, 5),
		strategies_factory(Pool::HUGE_OBJECT, 5)
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
