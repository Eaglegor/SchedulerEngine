#include "ChainVRPSolverLoader.h"

#include <Persistence/Utils/PtreeDeserializer.h>
#include <Persistence/StrategyLoaders/JSONStrategyLoader/LoaderImpl.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
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

	VRPSolver& ChainVRPSolverLoader::load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager)
	{
		ChainVRPSolver& chain_solver = algorithms_manager.createAlgorithm<ChainVRPSolver>();

		ChainVRPSolverDesc desc = PtreeDeserializer<ChainVRPSolverDesc>()(settings);
		for(const auto& solver_desc : desc.solvers)
		{
			VRPSolver& solver = loader_instance.loadVRPSolver(solver_desc);
			chain_solver.appendSolver(solver);
		}

		return chain_solver;
	}

	const char* ChainVRPSolverLoader::getName() const
	{
		return ChainVRPSolver::staticGetName();
	}
}
