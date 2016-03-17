#pragma once

#include <Persistence/Utils/PtreeDeserializer.h>
#include "TimeWindowDesc.h"

namespace Scheduler
{
	struct ScheduleShiftDesc
	{
		TimeWindowDesc time_window;
		boost::optional<std::string> start_location;
		std::string depot_location;
		boost::optional<std::string> end_location;
	};

	template<>
	struct PtreeDeserializer<ScheduleShiftDesc>
	{
		ScheduleShiftDesc operator()(const boost::property_tree::ptree &tree)
		{
			ScheduleShiftDesc desc;
			REQUIRED_SET(tree, desc, time_window);
			OPTIONAL_SET(tree, desc, start_location);
			REQUIRED_SET(tree, desc, depot_location);
			OPTIONAL_SET(tree, desc, end_location);
			return desc;
		}
	};
}