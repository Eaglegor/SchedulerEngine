#pragma once

#include "PtreeDeserializer.h"

namespace Scheduler
{
	struct LocationDesc
	{
		std::string name;
		float latitude;
		float longitude;
	};

	template<>
	struct PtreeDeserializer<LocationDesc>
	{
		LocationDesc operator()(const boost::property_tree::ptree &tree)
		{
			LocationDesc desc;
			REQUIRED_SET(tree, desc, name);
			REQUIRED_SET(tree, desc, latitude);
			REQUIRED_SET(tree, desc, longitude);
			return desc;
		}
	};
}
