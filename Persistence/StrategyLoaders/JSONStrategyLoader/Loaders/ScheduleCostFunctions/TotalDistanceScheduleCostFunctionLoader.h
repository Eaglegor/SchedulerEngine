#pragma once

#include "ScheduleCostFunctionLoader.h"

namespace Scheduler
{
	class TotalDistanceScheduleCostFunctionLoader : public ScheduleCostFunctionLoader
	{
	public:
		virtual ScheduleCostFunction* load(const boost::property_tree::ptree &settings, LoaderImpl* loader_instance) override;

		virtual const char* getName() const override;
	};
}