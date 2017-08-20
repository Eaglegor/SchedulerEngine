#include "LoaderImpl.h"

#include "Loaders/VRPSolvers/ChainVRPSolverLoader.h"
#include "Loaders/VRPSolvers/TSPOnlyVRPSolverLoader.h"
#include "Loaders/VRPSolvers/TransparentVRPSolverLoader.h"

#include "Loaders/TSPSolvers/SimpleTwoOptTSPSolverLoader.h"

#include "Loaders/RunCostFunctions/TotalDistanceRunCostFunctionLoader.h"

#include "Loaders/ScheduleCostFunctions/TotalDistanceScheduleCostFunctionLoader.h"

#include "Loaders/SceneCostFunctions/TotalDistanceSceneCostFunctionLoader.h"
#include "UnknownAlgorithmException.h"

namespace Scheduler
{
	LoaderImpl::LoaderImpl(AlgorithmsManager& algorithms_manager)
	    : algorithms_manager(algorithms_manager)
	{
		registerVRPSolverLoader<ChainVRPSolverLoader>( );
		registerVRPSolverLoader<TransparentVRPSolverLoader>( );
		registerVRPSolverLoader<TSPOnlyVRPSolverLoader>( );

		registerTSPSolverLoader<SimpleTwoOptTSPSolverLoader>( );

		registerRunCostFunctionLoader<TotalDistanceRunCostFunctionLoader>( );

		registerScheduleCostFunctionLoader<TotalDistanceScheduleCostFunctionLoader>( );

		registerSceneCostFunctionLoader<TotalDistanceSceneCostFunctionLoader>( );
	}

	VRPSolver& LoaderImpl::loadVRPSolver(const boost::property_tree::ptree& props)
	{
		std::string solver_type = props.get<std::string>("type");
		auto iter               = vrp_solver_loaders.find(solver_type);
		assert(iter != vrp_solver_loaders.end( ));
		if(iter == vrp_solver_loaders.end( )) throw UnknownAlgorithmException(solver_type);

		boost::property_tree::ptree settings = props.get_child("settings");
		return iter->second->load(settings, *this, algorithms_manager);
	}

	TSPSolver& LoaderImpl::loadTSPSolver(const boost::property_tree::ptree& props)
	{
		std::string solver_type = props.get<std::string>("type");
		auto iter               = tsp_solver_loaders.find(solver_type);
		assert(iter != tsp_solver_loaders.end( ));
		if(iter == tsp_solver_loaders.end( )) throw UnknownAlgorithmException(solver_type);

		boost::property_tree::ptree settings = props.get_child("settings");
		return iter->second->load(settings, *this, algorithms_manager);
	}

	RunCostFunction& LoaderImpl::loadRunCostFunction(const boost::property_tree::ptree& props)
	{
		std::string cf_type = props.get<std::string>("type");
		auto iter           = run_cost_function_loaders.find(cf_type);
		assert(iter != run_cost_function_loaders.end( ));
		if(iter == run_cost_function_loaders.end( )) throw UnknownAlgorithmException(cf_type);

		boost::property_tree::ptree settings = props.get_child("settings");
		return iter->second->load(settings, *this, algorithms_manager);
	}

	ScheduleCostFunction& LoaderImpl::loadScheduleCostFunction(const boost::property_tree::ptree& props)
	{
		std::string cf_type = props.get<std::string>("type");
		auto iter           = schedule_cost_function_loaders.find(cf_type);
		assert(iter != schedule_cost_function_loaders.end( ));
		if(iter == schedule_cost_function_loaders.end( )) throw UnknownAlgorithmException(cf_type);

		boost::property_tree::ptree settings = props.get_child("settings");
		return iter->second->load(settings, *this, algorithms_manager);
	}

	SceneCostFunction& LoaderImpl::loadSceneCostFunction(const boost::property_tree::ptree& props)
	{
		std::string cf_type = props.get<std::string>("type");
		auto iter           = scene_cost_function_loaders.find(cf_type);
		assert(iter != scene_cost_function_loaders.end( ));
		if(iter == scene_cost_function_loaders.end( )) throw UnknownAlgorithmException(cf_type);

		boost::property_tree::ptree settings = props.get_child("settings");
		return iter->second->load(settings, *this, algorithms_manager);
	}
}
