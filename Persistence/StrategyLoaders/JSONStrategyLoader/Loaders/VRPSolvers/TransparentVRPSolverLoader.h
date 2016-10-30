#pragma once

#include "VRPSolverLoader.h"

namespace Scheduler
{
	class TransparentVRPSolverLoader : public VRPSolverLoader
	{
	public:
		virtual VRPSolver& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) override;

		virtual const char* getName() const override;
	};
}