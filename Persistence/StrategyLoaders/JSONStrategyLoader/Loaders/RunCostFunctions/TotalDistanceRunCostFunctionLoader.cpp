#include "TotalDistanceRunCostFunctionLoader.h"

#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceRunCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	RunCostFunction& TotalDistanceRunCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		return algorithms_manager.createCostFunction<TotalDistanceRunCostFunction>( );
	}

	const char* TotalDistanceRunCostFunctionLoader::getName( ) const
	{
		return TotalDistanceRunCostFunction::staticGetName( );
	}
}
