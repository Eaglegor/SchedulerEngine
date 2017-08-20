#pragma once

#include "RunCostFunctionLoader.h"

namespace Scheduler
{
	class TotalDistanceRunCostFunctionLoader : public RunCostFunctionLoader
	{
	public:
		virtual RunCostFunction& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) override;

		virtual const char* getName( ) const override;
	};
}