#pragma once

#include <Engine/Utils/String.h>
#include <istream>
#include <string>



namespace Scheduler
{
	class AlgorithmsManager;
	class VRPSolver;

	class JSONStrategyLoader
	{
	public:
		explicit JSONStrategyLoader(AlgorithmsManager& strategies_manager);

		const VRPSolver& loadVRPStrategy(std::istream& stream) const;
		const VRPSolver& loadVRPStrategy(const String& filename) const;

	private:
		AlgorithmsManager& algorithms_manager;
	};
}