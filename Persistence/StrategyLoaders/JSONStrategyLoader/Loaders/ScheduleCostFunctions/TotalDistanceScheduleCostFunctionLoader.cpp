#include "TotalDistanceScheduleCostFunctionLoader.h"

#include <Engine/AlgorithmsManager/CostFunction.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceScheduleCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	ScheduleCostFunction& TotalDistanceScheduleCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		return algorithms_manager.createCostFunction<TotalDistanceScheduleCostFunction>();
	}

	const char* TotalDistanceScheduleCostFunctionLoader::getName() const
	{
		return TotalDistanceScheduleCostFunction::staticGetName();
	}
}
