#include "TotalDistanceSceneCostFunctionLoader.h"

#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceSceneCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	SceneCostFunction& TotalDistanceSceneCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		return algorithms_manager.createCostFunction<TotalDistanceSceneCostFunction>();
	}

	const char* TotalDistanceSceneCostFunctionLoader::getName() const
	{
		return TotalDistanceSceneCostFunction::staticGetName();
	}
}
