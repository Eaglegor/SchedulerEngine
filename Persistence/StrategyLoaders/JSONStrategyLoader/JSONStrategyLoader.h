#pragma once

#include <istream>
#include <string>

#include <JSONStrategyLoader_export.h>

namespace Scheduler
{
	class StrategiesManager;
	class Strategy;

	class JSONSTRATEGYLOADER_EXPORT JSONStrategyLoader
	{
	public:
		JSONStrategyLoader(StrategiesManager* strategies_manager);

		Strategy* loadStrategy(std::istream &stream) const;
		Strategy* loadStrategy(const std::string& filename) const;

	private:
		StrategiesManager* strategies_manager;
	};
}