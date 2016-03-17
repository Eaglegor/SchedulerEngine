#pragma once

#include <istream>
#include <string>
#include <unordered_map>
#include <memory>

#include "Loaders/VRPSolvers/VRPSolverLoader.h"
#include "Loaders/TSPSolvers/TSPSolverLoader.h"

namespace Scheduler
{
	class StrategiesManager;
	class Strategy;

	class JSONStrategyLoader
	{
	public:
		JSONStrategyLoader(StrategiesManager* strategies_manager);

		Strategy* loadStrategy(std::istream &stream);
		Strategy* loadStrategy(const std::string& filename);

	private:
		StrategiesManager* strategies_manager;

		std::unordered_map<std::string, std::shared_ptr<VRPSolverLoader>> vrp_solver_loaders;
		std::unordered_map<std::string, std::shared_ptr<TSPSolverLoader>> tsp_solver_loaders;
	};
}