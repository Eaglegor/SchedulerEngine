#pragma once

#include <Persistence/Utils/PtreeDeserializer.h>
#include <boost/optional.hpp>

namespace Scheduler
{
	struct LoaderSettings
	{
		boost::optional<size_t> load_dimensions;
		boost::optional<std::string> default_time_format;
		boost::optional<std::string> default_duration_format;
	};

	template <>
	struct PtreeDeserializer<LoaderSettings>
	{
		LoaderSettings operator( )(const boost::property_tree::ptree& tree)
		{
			LoaderSettings desc;
			OPTIONAL_SET_OR_DEFAULT(tree, desc, load_dimensions, 4);
			OPTIONAL_SET(tree, desc, default_time_format);
			OPTIONAL_SET(tree, desc, default_duration_format);
			return desc;
		}
	};
}