#pragma once

#include "PtreeDeserializer.h"

namespace Scheduler
{
	struct RoutingProfileDesc
	{
		std::string vehicle_type;
		float average_speed;
	};

	template<>
	struct PtreeDeserializer<RoutingProfileDesc>
	{
		RoutingProfileDesc operator()(const boost::property_tree::ptree &tree)
		{
			RoutingProfileDesc desc;
			REQUIRED_SET(tree, desc, vehicle_type);
			REQUIRED_SET(tree, desc, average_speed);
			return desc;
		}
	};
}