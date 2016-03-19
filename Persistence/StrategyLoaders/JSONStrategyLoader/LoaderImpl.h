#pragma once

#include <boost/property_tree/ptree.hpp>
#include <unordered_map>
#include <memory>

#include "Loaders/TSPSolvers/TSPSolverLoader.h"
#include "Loaders/VRPSolvers/VRPSolverLoader.h"
#include "Loaders/RunCostFunctions/RunCostFunctionLoader.h"
#include "Loaders/ScheduleCostFunctions/ScheduleCostFunctionLoader.h"
#include "Loaders/SceneCostFunctions/SceneCostFunctionLoader.h"

namespace Scheduler
{
	class Strategy;

	class LoaderImpl
	{
	public:
		LoaderImpl(Strategy* strategy);

		VRPSolver* loadVRPSolver(const boost::property_tree::ptree& props);
		TSPSolver* loadTSPSolver(const boost::property_tree::ptree& props);

		RunCostFunction* loadRunCostFunction(const boost::property_tree::ptree& props);
		ScheduleCostFunction* loadScheduleCostFunction(const boost::property_tree::ptree& props);
		SceneCostFunction* loadSceneCostFunction(const boost::property_tree::ptree& props);

		Strategy* getStrategy() const;

	private:
		template<typename T>
		void registerVRPSolverLoader()
		{
			auto loader = std::make_shared<T>();
			vrp_solver_loaders.emplace(loader->getName(), loader);
		}

		template<typename T>
		void registerTSPSolverLoader()
		{
			auto loader = std::make_shared<T>();
			tsp_solver_loaders.emplace(loader->getName(), loader);
		}

		template<typename T>
		void registerRunCostFunctionLoader()
		{
			auto loader = std::make_shared<T>();
			run_cost_function_loaders.emplace(loader->getName(), loader);
		}

		template<typename T>
		void registerScheduleCostFunctionLoader()
		{
			auto loader = std::make_shared<T>();
			schedule_cost_function_loaders.emplace(loader->getName(), loader);
		}

		template<typename T>
		void registerSceneCostFunctionLoader()
		{
			auto loader = std::make_shared<T>();
			scene_cost_function_loaders.emplace(loader->getName(), loader);
		}

		Strategy* strategy;

		std::unordered_map<std::string, std::shared_ptr<VRPSolverLoader>> vrp_solver_loaders;
		std::unordered_map<std::string, std::shared_ptr<TSPSolverLoader>> tsp_solver_loaders;

		std::unordered_map<std::string, std::shared_ptr<RunCostFunctionLoader>> run_cost_function_loaders;
		std::unordered_map<std::string, std::shared_ptr<ScheduleCostFunctionLoader>> schedule_cost_function_loaders;
		std::unordered_map<std::string, std::shared_ptr<SceneCostFunctionLoader>> scene_cost_function_loaders;
	};
}
