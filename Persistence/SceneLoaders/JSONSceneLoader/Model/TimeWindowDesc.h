#pragma once

#include <Engine/Utils/Collections/Algorithms.h>
#include <Persistence/Utils/PtreeDeserializer.h>
#include <string>

namespace Scheduler
{
	struct TimeWindowDesc
	{
		boost::optional<std::string> format;
		std::string start_time;
		std::string end_time;
	};

	template <>
	struct PtreeDeserializer<TimeWindowDesc>
	{
		TimeWindowDesc operator( )(const boost::property_tree::ptree& tree)
		{
			TimeWindowDesc desc;
			OPTIONAL_SET(tree, desc, format);
			REQUIRED_SET(tree, desc, start_time);
			REQUIRED_SET(tree, desc, end_time);
			return desc;
		}
	};
}