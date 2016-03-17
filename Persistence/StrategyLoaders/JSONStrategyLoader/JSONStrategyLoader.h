#pragma once

#include <istream>
#include <string>

namespace Scheduler
{
	class StrategiesManager;
	class Strategy;

	class JSONStrategyLoader
	{
	public:
		JSONStrategyLoader(StrategiesManager* strategies_manager);

		Strategy* loadStrategy(std::istream &stream);
		Strategy* loadStrategy(const std::string& filename);

	private:
		StrategiesManager* strategies_manager;
	};
}