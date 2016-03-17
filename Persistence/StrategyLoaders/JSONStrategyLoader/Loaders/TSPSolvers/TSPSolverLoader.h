#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class TSPSolver;

	class TSPSolverLoader
	{
	public:
		virtual ~TSPSolverLoader() {}

		TSPSolver* load(const boost::property_tree::ptree &settings);
	};
}