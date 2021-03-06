#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class SceneCostFunction;
	class LoaderImpl;
	class AlgorithmsManager;

	class SceneCostFunctionLoader
	{
	public:
		virtual ~SceneCostFunctionLoader( )
		{
		}

		virtual SceneCostFunction& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) = 0;

		virtual const char* getName( ) const = 0;
	};
}