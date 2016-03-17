#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Schedule;

	class ScheduleCostFunction
	{
	public:
		virtual Cost calculateCost(const Schedule* schedule) = 0;
	};
}