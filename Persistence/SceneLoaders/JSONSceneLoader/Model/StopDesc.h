#pragma once

#include "PtreeDeserializer.h"
#include "TimeWindowDesc.h"

namespace Scheduler
{
	struct StopDesc
	{
		std::string operation;
		TimeWindowDesc allocation_time;
	};

	template<>
	struct PtreeDeserializer<StopDesc>
	{
		StopDesc operator()(const boost::property_tree::ptree &tree)
		{
			StopDesc desc;
			REQUIRED_SET(tree, desc, operation);
			REQUIRED_SET(tree, desc, allocation_time);
			return desc;
		}
	};
}