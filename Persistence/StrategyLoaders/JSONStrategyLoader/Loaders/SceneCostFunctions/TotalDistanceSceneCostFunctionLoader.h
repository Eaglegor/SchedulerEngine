#pragma once

#include "SceneCostFunctionLoader.h"

namespace Scheduler
{
	class TotalDistanceSceneCostFunctionLoader : public SceneCostFunctionLoader
	{
	public:
		virtual SceneCostFunction* load(const boost::property_tree::ptree &settings, LoaderImpl* loader_instance) override;
	};
}