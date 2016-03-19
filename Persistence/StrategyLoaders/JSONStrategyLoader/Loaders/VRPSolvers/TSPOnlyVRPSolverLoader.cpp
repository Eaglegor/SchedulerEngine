#include "TSPOnlyVRPSolverLoader.h"

#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct TSPOnlyVRPSolverDesc
	{
	public:
		boost::property_tree::ptree tsp_solver;
	};

	template<>
	struct PtreeDeserializer<TSPOnlyVRPSolverDesc>
	{
		TSPOnlyVRPSolverDesc operator()(const boost::property_tree::ptree &tree)
		{
			TSPOnlyVRPSolverDesc desc;
			REQUIRED_SET(tree, desc, tsp_solver);
			return desc;
		}
	};

	VRPSolver* TSPOnlyVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		TSPOnlyVRPSolverDesc desc = PtreeDeserializer<TSPOnlyVRPSolverDesc>()(settings);
		TSPSolver* tsp_solver = loader_instance->loadTSPSolver(desc.tsp_solver);

		TSPOnlyVRPSolver* tsp_only_vrp_solver = loader_instance->getStrategy()->createVRPSolver<TSPOnlyVRPSolver>(tsp_solver);

		return tsp_only_vrp_solver;
	}

	const char* TSPOnlyVRPSolverLoader::getName() const
	{
		return TSPOnlyVRPSolver::staticGetName();
	}
}
