#pragma once

#include "PtreeDeserializer.h"
#include "StopDesc.h"

namespace Scheduler
{
	struct RunDesc
	{
		std::string vehicle;
		std::string start_location;
		std::string end_location;
		std::vector<StopDesc> stops;
	};

	template<>
	struct PtreeDeserializer<RunDesc>
	{
		RunDesc operator()(const boost::property_tree::ptree &tree)
		{
			RunDesc desc;
			REQUIRED_SET(tree, desc, vehicle);
			REQUIRED_SET(tree, desc, start_location);
			REQUIRED_SET(tree, desc, end_location);
			REQUIRED_SET(tree, desc, stops);
			return desc;
		}
	};
}