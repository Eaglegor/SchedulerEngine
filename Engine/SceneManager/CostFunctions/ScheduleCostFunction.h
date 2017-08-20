#pragma once

#include <Engine/AlgorithmsManager/CostFunction.h>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Schedule;

	class ScheduleCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Schedule& schedule) const = 0;
	};
}