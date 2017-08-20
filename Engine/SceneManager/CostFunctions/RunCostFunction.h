#pragma once

#include <Engine/AlgorithmsManager/CostFunction.h>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Run;

	class RunCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Run& run) const = 0;
	};
}