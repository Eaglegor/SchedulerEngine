#pragma once

#include <Engine/Concepts/Cost.h>
#include <Engine/AlgorithmsManager/CostFunction.h>

namespace Scheduler
{
	class Run;

	class RunCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Run& run) const = 0;
	};
}