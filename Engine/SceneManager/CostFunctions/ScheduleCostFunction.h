#pragma once

#include <Engine/Concepts/Cost.h>
#include <Engine/AlgorithmsManager/CostFunction.h>

namespace Scheduler
{
	class Schedule;

	class ScheduleCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Schedule& schedule) const = 0;
	};
}