#pragma once

#include <Persistence/Utils/PtreeDeserializer.h>
#include "LoaderSettings.h"
#include "LocationDesc.h"
#include "OrderDesc.h"
#include "FleetDesc.h"
#include "ScheduleDesc.h"

namespace Scheduler
{
	struct SceneDesc
	{
		boost::optional<LoaderSettings> settings;
		std::vector<LocationDesc> locations;
		std::vector<OrderDesc> orders;
		std::vector<OperationDesc> free_operations;
		FleetDesc fleet;
		std::vector<ScheduleDesc> schedules;
	};

	template<>
	struct PtreeDeserializer<SceneDesc>
	{
		SceneDesc operator()(const boost::property_tree::ptree &tree)
		{
			SceneDesc desc;
			OPTIONAL_SET(tree, desc, settings);
			REQUIRED_SET(tree, desc, locations);
			REQUIRED_SET(tree, desc, orders);
			OPTIONAL_SET(tree, desc, free_operations);
			REQUIRED_SET(tree, desc, fleet);
			OPTIONAL_SET(tree, desc, schedules);
			return desc;
		}
	};
}