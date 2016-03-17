#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class ScheduleCostFunction;
	class LoaderImpl;

	class ScheduleCostFunctionLoader
	{
	public:
		virtual ~ScheduleCostFunctionLoader() {}

		virtual ScheduleCostFunction* load(const boost::property_tree::ptree &settings, LoaderImpl* loader_instance) = 0;
	};
}