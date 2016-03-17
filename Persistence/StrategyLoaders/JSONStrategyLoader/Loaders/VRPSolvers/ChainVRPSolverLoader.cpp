#include "ChainVRPSolverLoader.h"

#include <Persistence/Utils/PtreeDeserializer.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <Engine/Algorithms/VRPSolvers/Utilitary/Chain/ChainVRPSolver.h>

namespace Scheduler
{
	struct ChainVRPSolverDesc
	{
	public:
		std::vector<boost::property_tree::ptree> solvers;
	};

	template<>
	struct PtreeDeserializer<ChainVRPSolverDesc>
	{
		ChainVRPSolverDesc operator()(const boost::property_tree::ptree &tree)
		{
			ChainVRPSolverDesc desc;
			OPTIONAL_SET(tree, desc, solvers);
			return desc;
		}
	};

	VRPSolver* ChainVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl* loader_instance)
	{
		ChainVRPSolver* chain_solver = loader_instance->getStrategy()->createVRPSolver<ChainVRPSolver>();

		ChainVRPSolverDesc desc = PtreeDeserializer<ChainVRPSolverDesc>()(settings);
		for(const auto& solver_desc : desc.solvers)
		{
			VRPSolver* solver = loader_instance->loadVRPSolver(solver_desc);
			assert(solver);
			chain_solver->appendSolver(solver);
		}

		return chain_solver;
	}
}
