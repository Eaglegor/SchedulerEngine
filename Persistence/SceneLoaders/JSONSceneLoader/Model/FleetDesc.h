#pragma once

#include "PerformerDesc.h"
#include "VehicleDesc.h"
#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct FleetDesc
	{
		std::vector<PerformerDesc> performers;
		std::vector<VehicleDesc> vehicles;
	};

	template<>
	struct PtreeDeserializer<FleetDesc>
	{
		FleetDesc operator()(const boost::property_tree::ptree &tree)
		{
			FleetDesc desc;
			REQUIRED_SET(tree, desc, performers);
			REQUIRED_SET(tree, desc, vehicles);
			return desc;
		}
	};
}