#include "TotalDistanceSceneCostFunctionLoader.h"

#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceSceneCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	SceneCostFunction* TotalDistanceSceneCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		TotalDistanceSceneCostFunction* scene_cost_function = loader_instance->getStrategy()->createSceneCostFunction<TotalDistanceSceneCostFunction>();
		return scene_cost_function;
	}
}
