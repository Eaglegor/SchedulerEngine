#pragma once

#include <Persistence/Utils/PtreeDeserializer.h>

namespace Scheduler
{
	struct SiteDesc
	{
		std::string name;
		float latitude;
		float longitude;
	};

	template <>
	struct PtreeDeserializer<SiteDesc>
	{
		SiteDesc operator( )(const boost::property_tree::ptree& tree)
		{
			SiteDesc desc;
			REQUIRED_SET(tree, desc, name);
			REQUIRED_SET(tree, desc, latitude);
			REQUIRED_SET(tree, desc, longitude);
			return desc;
		}
	};
}
