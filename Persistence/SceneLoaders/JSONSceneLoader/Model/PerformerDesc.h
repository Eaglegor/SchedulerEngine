#pragma once

#include "PtreeDeserializer.h"
#include "TimeWindowDesc.h"

namespace Scheduler
{
	struct PerformerDesc
	{
		std::string name;
		std::vector<TimeWindowDesc> availability_windows;
		std::vector<std::string> skills;
		boost::optional<float> hour_cost;
		boost::optional<float> activation_cost;
	};

	template<>
	struct PtreeDeserializer<PerformerDesc>
	{
		PerformerDesc operator()(const boost::property_tree::ptree &tree)
		{
			PerformerDesc desc;
			REQUIRED_SET(tree, desc, name);
			REQUIRED_SET(tree, desc, availability_windows);
			OPTIONAL_SET(tree, desc, skills);
			OPTIONAL_SET_OR_DEFAULT(tree, desc, hour_cost, 0.0f);
			OPTIONAL_SET_OR_DEFAULT(tree, desc, activation_cost, 0.0f);
			return desc;
		}
	};
}