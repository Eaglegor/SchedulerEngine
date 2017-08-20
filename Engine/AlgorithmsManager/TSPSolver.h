#pragma once

#include "Algorithm.h"

namespace Scheduler
{
	class Run;
	class Schedule;

	class TSPSolver : public Algorithm
	{
	public:
		virtual void optimize(Schedule& schedule) const = 0;
		virtual void optimize(Run& run) const           = 0;
	};
}