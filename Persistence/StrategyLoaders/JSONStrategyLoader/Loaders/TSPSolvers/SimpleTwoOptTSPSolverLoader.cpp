#include "SimpleTwoOptTSPSolverLoader.h"

#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/TSPSolvers/SimpleTwoOpt/SimpleTwoOptTSPSolver.h>

namespace Scheduler
{
	TSPSolver* SimpleTwoOptTSPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		SimpleTwoOptTSPSolver* simple_two_opt_tsp_solver = loader_instance->getStrategy()->createTSPSolver<SimpleTwoOptTSPSolver>();

		return simple_two_opt_tsp_solver;
	}
}
