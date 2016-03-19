#include "TotalDistanceScheduleCostFunctionLoader.h"

#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/CostFunctions/TotalDistance/TotalDistanceScheduleCostFunction.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	ScheduleCostFunction* TotalDistanceScheduleCostFunctionLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		TotalDistanceScheduleCostFunction* schedule_cost_function = loader_instance->getStrategy()->createScheduleCostFunction<TotalDistanceScheduleCostFunction>();
		return schedule_cost_function;
	}

	const char* TotalDistanceScheduleCostFunctionLoader::getName() const
	{
		return TotalDistanceScheduleCostFunction::staticGetName();
	}
}
