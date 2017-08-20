#pragma once

#include "RunDesc.h"
#include "ScheduleShiftDesc.h"
#include "TimeWindowDesc.h"
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct ScheduleDesc
	{
		std::string performer;
		ScheduleShiftDesc shift;
		std::vector<RunDesc> runs;
	};

	template <>
	struct PtreeDeserializer<ScheduleDesc>
	{
		ScheduleDesc operator( )(const boost::property_tree::ptree& tree)
		{
			ScheduleDesc desc;
			REQUIRED_SET(tree, desc, performer);
			REQUIRED_SET(tree, desc, shift);
			OPTIONAL_SET(tree, desc, runs);
			return desc;
		}
	};
}