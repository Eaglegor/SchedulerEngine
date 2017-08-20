#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class Strategy;
	class TSPSolver;
	class LoaderImpl;
	class AlgorithmsManager;

	class TSPSolverLoader
	{
	public:
		virtual ~TSPSolverLoader( )
		{
		}

		virtual TSPSolver& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) = 0;

		virtual const char* getName( ) const = 0;
	};
}