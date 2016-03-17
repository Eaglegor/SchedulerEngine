#include "JSONStrategyLoader.h"

#include <assert.h>
#include <Engine/StrategiesManager/StrategiesManager.h>
#include <Engine/StrategiesManager/Strategy.h>
#include <fstream>

#include "LoaderImpl.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Scheduler
{
	JSONStrategyLoader::JSONStrategyLoader(StrategiesManager * strategies_manager):
	strategies_manager(strategies_manager)
	{
	}

	Strategy * JSONStrategyLoader::loadStrategy(std::istream & stream) const
	{
		assert(strategies_manager);
		assert(stream.good());

		Strategy* strategy = strategies_manager->createStrategy();

		LoaderImpl loader_impl(strategy);

		boost::property_tree::ptree desc;
		boost::property_tree::read_json(stream, desc);

		const boost::property_tree::ptree& child = desc.get_child("root_vrp_solver");

		VRPSolver* root_solver = loader_impl.loadVRPSolver(child);

		strategy->setRootVRPSolver(root_solver);

		return strategy;
	}

	Strategy * JSONStrategyLoader::loadStrategy(const std::string & filename) const
	{
		std::ifstream file;
		file.open(filename);

		assert(file.is_open());

		return loadStrategy(file);
	}
}