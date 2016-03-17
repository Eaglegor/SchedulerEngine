#pragma once

#include <boost/property_tree/ptree_fwd.hpp>

namespace Scheduler
{
	class VRPSolver;

	class VRPSolverLoader
	{
	public:
		virtual ~VRPSolverLoader() {}

		VRPSolver* load(const boost::property_tree::ptree &settings);
	};
}