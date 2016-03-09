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
		std::vector<OperationDesc> operations;
		boost::optional<std::string> start_operation;
		std::vector<std::string>  work_operations;
		boost::optional<std::string>  end_operation;
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
			REQUIRED_SET(tree, desc, operations);
			OPTIONAL_SET(tree, desc, start_operation);
			OPTIONAL_SET(tree, desc, work_operations);
			OPTIONAL_SET(tree, desc, end_operation);
			return desc;
		}
	};
}