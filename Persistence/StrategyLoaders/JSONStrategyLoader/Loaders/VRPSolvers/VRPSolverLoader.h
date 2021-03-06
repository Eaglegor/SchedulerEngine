#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class Strategy;
	class VRPSolver;
	class LoaderImpl;
	class AlgorithmsManager;

	class VRPSolverLoader
	{
	public:
		virtual ~VRPSolverLoader( )
		{
		}

		virtual VRPSolver& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) = 0;
		virtual const char* getName( ) const = 0;
	};
}