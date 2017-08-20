#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class RunCostFunction;
	class LoaderImpl;
	class AlgorithmsManager;

	class RunCostFunctionLoader
	{
	public:
		virtual ~RunCostFunctionLoader( )
		{
		}

		virtual RunCostFunction& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) = 0;

		virtual const char* getName( ) const = 0;
	};
}