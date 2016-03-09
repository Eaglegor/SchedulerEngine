#pragma once

#include "PtreeDeserializer.h"
#include "TimeWindowDesc.h"
#include "RoutingProfileDesc.h"
#include <vector>
#include <boost/optional.hpp>

namespace Scheduler
{
	struct VehicleDesc
	{
		std::string name;
		std::vector<TimeWindowDesc> availability_windows;
		std::vector<std::string> attributes;
		std::vector<float> capacity;
		boost::optional<float> hour_cost;
		boost::optional<float> distance_unit_cost;
		boost::optional<float> activation_cost;
		RoutingProfileDesc routing_profile;
	};

	template<>
	struct PtreeDeserializer<VehicleDesc>
	{
		VehicleDesc operator()(const boost::property_tree::ptree &tree)
		{
			VehicleDesc desc;
			REQUIRED_SET(tree, desc, name);
			REQUIRED_SET(tree, desc, availability_windows);
			REQUIRED_SET(tree, desc, attributes);
			REQUIRED_SET(tree, desc, capacity);
			OPTIONAL_SET_OR_DEFAULT(tree, desc, hour_cost, 0);
			OPTIONAL_SET_OR_DEFAULT(tree, desc, distance_unit_cost, 0);
			OPTIONAL_SET_OR_DEFAULT(tree, desc, activation_cost, 0);
			REQUIRED_SET(tree, desc, routing_profile);
			return desc;
		}
	};
}