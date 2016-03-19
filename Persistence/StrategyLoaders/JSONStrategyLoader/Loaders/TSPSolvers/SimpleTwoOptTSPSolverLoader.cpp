#include "SimpleTwoOptTSPSolverLoader.h"

#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/TSPSolvers/SimpleTwoOpt/SimpleTwoOptTSPSolver.h>
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct SimpleTwoOptTSPSolverDesc
	{
	public:
		boost::property_tree::ptree schedule_cost_function;
	};

	template<>
	struct PtreeDeserializer<SimpleTwoOptTSPSolverDesc>
	{
		SimpleTwoOptTSPSolverDesc operator()(const boost::property_tree::ptree &tree)
		{
			SimpleTwoOptTSPSolverDesc desc;
			REQUIRED_SET(tree, desc, schedule_cost_function);
			return desc;
		}
	};

	const char* SimpleTwoOptTSPSolverLoader::getName() const
	{
		return SimpleTwoOptTSPSolver::staticGetName();
	}

	TSPSolver* SimpleTwoOptTSPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		SimpleTwoOptTSPSolver* simple_two_opt_tsp_solver = loader_instance->getStrategy()->createTSPSolver<SimpleTwoOptTSPSolver>();

		SimpleTwoOptTSPSolverDesc desc = PtreeDeserializer<SimpleTwoOptTSPSolverDesc>()(settings);
		ScheduleCostFunction* cost_function = loader_instance->loadScheduleCostFunction(desc.schedule_cost_function);

		return simple_two_opt_tsp_solver;
	}
}
