#include "StrategiesManager.h"
#include <Utils/Collections/Algorithms.h>
#include <assert.h>
#include "Strategy.h"

namespace Scheduler
{
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
