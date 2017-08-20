#pragma once

#include "StopDesc.h"
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct RunDesc
	{
		std::string vehicle;
		std::string start_location;
		std::string end_location;
		std::vector<StopDesc> start_operations;
		std::vector<StopDesc> work_operations;
		std::vector<StopDesc> end_operations;
	};

	template <>
	struct PtreeDeserializer<RunDesc>
	{
		RunDesc operator( )(const boost::property_tree::ptree& tree)
		{
			RunDesc desc;
			REQUIRED_SET(tree, desc, vehicle);
			REQUIRED_SET(tree, desc, start_location);
			REQUIRED_SET(tree, desc, end_location);
			OPTIONAL_SET(tree, desc, start_operations);
			OPTIONAL_SET(tree, desc, work_operations);
			OPTIONAL_SET(tree, desc, end_operations);
			return desc;
		}
	};
}