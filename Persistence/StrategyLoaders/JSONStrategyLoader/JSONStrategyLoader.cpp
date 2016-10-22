#include "JSONStrategyLoader.h"

#include <assert.h>
#include <Engine/AlgorithmsManager/AlgorithmsManager.h>
#include <fstream>

#include "LoaderImpl.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Scheduler
{
	JSONStrategyLoader::JSONStrategyLoader(AlgorithmsManager& algorithms_manager):
	algorithms_manager(algorithms_manager)
	{
	}

	const VRPSolver& JSONStrategyLoader::loadVRPStrategy(std::istream& stream) const
	{
		assert(stream.good());
		
		LoaderImpl loader_impl(algorithms_manager);

		boost::property_tree::ptree desc;
		boost::property_tree::read_json(stream, desc);

		const boost::property_tree::ptree& child = desc.get_child("root_vrp_solver");

		return loader_impl.loadVRPSolver(child);
	}

	const VRPSolver& JSONStrategyLoader::loadVRPStrategy(const String& filename) const
	{
		std::ifstream file;
		file.open(filename);

		assert(file.is_open());

		return loadVRPStrategy(file);
	}
}