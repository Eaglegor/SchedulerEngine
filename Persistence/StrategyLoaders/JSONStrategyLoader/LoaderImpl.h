#pragma once

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <unordered_map>

#include "Loaders/RunCostFunctions/RunCostFunctionLoader.h"
#include "Loaders/SceneCostFunctions/SceneCostFunctionLoader.h"
#include "Loaders/ScheduleCostFunctions/ScheduleCostFunctionLoader.h"
#include "Loaders/TSPSolvers/TSPSolverLoader.h"
#include "Loaders/VRPSolvers/VRPSolverLoader.h"

namespace Scheduler
{
	class AlgorithmsManager;

	class LoaderImpl
	{
	public:
		explicit LoaderImpl(AlgorithmsManager& algorithms_manager);

		VRPSolver& loadVRPSolver(const boost::property_tree::ptree& props);
		TSPSolver& loadTSPSolver(const boost::property_tree::ptree& props);

		RunCostFunction& loadRunCostFunction(const boost::property_tree::ptree& props);
		ScheduleCostFunction& loadScheduleCostFunction(const boost::property_tree::ptree& props);
		SceneCostFunction& loadSceneCostFunction(const boost::property_tree::ptree& props);

	private:
		template <typename T>
		void registerVRPSolverLoader( )
		{
			auto loader = std::make_shared<T>( );
			vrp_solver_loaders.emplace(loader->getName( ), loader);
		}

		template <typename T>
		void registerTSPSolverLoader( )
		{
			auto loader = std::make_shared<T>( );
			tsp_solver_loaders.emplace(loader->getName( ), loader);
		}

		template <typename T>
		void registerRunCostFunctionLoader( )
		{
			auto loader = std::make_shared<T>( );
			run_cost_function_loaders.emplace(loader->getName( ), loader);
		}

		template <typename T>
		void registerScheduleCostFunctionLoader( )
		{
			auto loader = std::make_shared<T>( );
			schedule_cost_function_loaders.emplace(loader->getName( ), loader);
		}

		template <typename T>
		void registerSceneCostFunctionLoader( )
		{
			auto loader = std::make_shared<T>( );
			scene_cost_function_loaders.emplace(loader->getName( ), loader);
		}

		AlgorithmsManager& algorithms_manager;

		std::unordered_map<std::string, std::shared_ptr<VRPSolverLoader>> vrp_solver_loaders;
		std::unordered_map<std::string, std::shared_ptr<TSPSolverLoader>> tsp_solver_loaders;

		std::unordered_map<std::string, std::shared_ptr<RunCostFunctionLoader>> run_cost_function_loaders;
		std::unordered_map<std::string, std::shared_ptr<ScheduleCostFunctionLoader>> schedule_cost_function_loaders;
		std::unordered_map<std::string, std::shared_ptr<SceneCostFunctionLoader>> scene_cost_function_loaders;
	};
}
