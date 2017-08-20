#include "TSPOnlyVRPSolverLoader.h"

#include <Engine/Algorithms/VRPSolvers/Utilitary/TSPOnly/TSPOnlyVRPSolver.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct TSPOnlyVRPSolverDesc
	{
	public:
		boost::property_tree::ptree tsp_solver;
	};

	template <>
	struct PtreeDeserializer<TSPOnlyVRPSolverDesc>
	{
		TSPOnlyVRPSolverDesc operator( )(const boost::property_tree::ptree& tree)
		{
			TSPOnlyVRPSolverDesc desc;
			REQUIRED_SET(tree, desc, tsp_solver);
			return desc;
		}
	};

	VRPSolver& TSPOnlyVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		TSPOnlyVRPSolverDesc desc = PtreeDeserializer<TSPOnlyVRPSolverDesc>( )(settings);
		TSPSolver& tsp_solver     = loader_instance.loadTSPSolver(desc.tsp_solver);

		return algorithms_manager.createAlgorithm<TSPOnlyVRPSolver>(tsp_solver);
	}

	const char* TSPOnlyVRPSolverLoader::getName( ) const
	{
		return TSPOnlyVRPSolver::staticGetName( );
	}
}
