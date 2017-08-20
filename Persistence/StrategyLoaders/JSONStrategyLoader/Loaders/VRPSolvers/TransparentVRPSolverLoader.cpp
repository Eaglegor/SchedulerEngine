#include "TransparentVRPSolverLoader.h"

#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>

namespace Scheduler
{
	VRPSolver& TransparentVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		return algorithms_manager.createAlgorithm<TransparentVRPSolver>( );
	}

	const char* TransparentVRPSolverLoader::getName( ) const
	{
		return TransparentVRPSolver::staticGetName( );
	}
}
