#pragma once

#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Run;

	class RunCostFunction
	{
	public:
		virtual ~RunCostFunction()
		{
		}

		virtual Cost calculateCost(const Run* run) = 0;
	};
}