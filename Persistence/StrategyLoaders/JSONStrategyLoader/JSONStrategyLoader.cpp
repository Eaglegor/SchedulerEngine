#include "JSONStrategyLoader.h"

#include <assert.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <fstream>

namespace Scheduler
{
	JSONStrategyLoader::JSONStrategyLoader(StrategiesManager * strategies_manager):
	strategies_manager(strategies_manager)
	{
	}

	Strategy * JSONStrategyLoader::loadStrategy(std::istream & stream)
	{
		assert(strategies_manager);
		assert(stream.good());

		Strategy* strategy = strategies_manager->createStrategy();

		return strategy;
	}

	Strategy * JSONStrategyLoader::loadStrategy(const std::string & filename)
	{
		std::ifstream file;
		file.open(filename);

		assert(file.is_open());

		return loadStrategy(file);
	}
}