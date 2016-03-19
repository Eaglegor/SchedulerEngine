#include "TotalDistanceRunCostFunctionLoader.h"

#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceRunCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	RunCostFunction* TotalDistanceRunCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		TotalDistanceRunCostFunction* run_cost_function = loader_instance->getStrategy()->createRunCostFunction<TotalDistanceRunCostFunction>();
		return run_cost_function;
	}

	const char* TotalDistanceRunCostFunctionLoader::getName() const
	{
		return TotalDistanceRunCostFunction::staticGetName();
	}
}
