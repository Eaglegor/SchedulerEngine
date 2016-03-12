#pragma once

#include "PtreeDeserializer.h"

#include "OperationDesc.h"

namespace Scheduler
{
	struct OrderDesc
	{
		std::string name;
		std::vector<std::string> vehicle_requirements;
		std::vector<std::string> performer_skill_requirements;
		boost::optional<OperationDesc> start_operation;
		std::vector<OperationDesc>  work_operations;
		boost::optional<OperationDesc>  end_operation;
	};

	template<>
	struct PtreeDeserializer<OrderDesc>
	{
		OrderDesc operator()(const boost::property_tree::ptree &tree)
		{
			OrderDesc desc;
			REQUIRED_SET(tree, desc, name);
			OPTIONAL_SET(tree, desc, vehicle_requirements);
			OPTIONAL_SET(tree, desc, performer_skill_requirements);
			OPTIONAL_SET(tree, desc, work_operations);
			OPTIONAL_SET(tree, desc, start_operation);
			OPTIONAL_SET(tree, desc, work_operations);
			OPTIONAL_SET(tree, desc, end_operation);
			return desc;
		}
	};
}