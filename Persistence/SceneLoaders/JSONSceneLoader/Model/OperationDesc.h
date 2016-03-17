#pragma once

#include <Persistence/Utils/PtreeDeserializer.h>

#include "TimeWindowDesc.h"

namespace Scheduler
{
	struct OperationDesc
	{
		std::string name;
		std::string location;
		std::vector<float> load;
		std::vector<TimeWindowDesc> time_windows;
		std::string duration;
		boost::optional<std::string> duration_format;
	};

	template<>
	struct PtreeDeserializer<OperationDesc>
	{
		OperationDesc operator()(const boost::property_tree::ptree &tree)
		{
			OperationDesc desc;
			REQUIRED_SET(tree, desc, name);
			REQUIRED_SET(tree, desc, location);
			REQUIRED_SET(tree, desc, load);
			REQUIRED_SET(tree, desc, time_windows);
			REQUIRED_SET(tree, desc, duration);
			OPTIONAL_SET(tree, desc, duration_format);
			return desc;
		}
	};
}