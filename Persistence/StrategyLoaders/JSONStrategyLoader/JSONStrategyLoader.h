#pragma once

#include <istream>
#include <string>
#include <Engine/Utils/String.h>

#include <JSONStrategyLoader_export.h>

namespace Scheduler
{
	class AlgorithmsManager;
	class VRPSolver;

	class JSONSTRATEGYLOADER_EXPORT JSONStrategyLoader
	{
	public:
		explicit JSONStrategyLoader(AlgorithmsManager& strategies_manager);

		const VRPSolver& loadVRPStrategy(std::istream &stream) const;
		const VRPSolver& loadVRPStrategy(const String &filename) const;

	private:
		AlgorithmsManager& algorithms_manager;
	};
}