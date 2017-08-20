#pragma once

#include "TSPSolverLoader.h"

namespace Scheduler
{
	class SimpleTwoOptTSPSolverLoader : public TSPSolverLoader
	{
	public:
		virtual TSPSolver& load(const boost::property_tree::ptree& settings, LoaderImpl& loader_instance, AlgorithmsManager& algorithms_manager) override;

		const char* getName( ) const override;
	};
}