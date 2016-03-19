#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Schedule;

	class ScheduleCostFunction
	{
	public:
		virtual ~ScheduleCostFunction()
		{
		}

		virtual Cost calculateCost(const Schedule* schedule) = 0;

		virtual const char* getName() const = 0;
	};
}