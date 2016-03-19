#include "TransparentVRPSolverLoader.h"

#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Transparent/TransparentVRPSolver.h>

namespace Scheduler
{
	VRPSolver* TransparentVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		TransparentVRPSolver* transparent_vrp_solver = loader_instance->getStrategy()->createVRPSolver<TransparentVRPSolver>();
		return transparent_vrp_solver;
	}

	const char* TransparentVRPSolverLoader::getName() const
	{
		return TransparentVRPSolver::staticGetName();
	}
}
