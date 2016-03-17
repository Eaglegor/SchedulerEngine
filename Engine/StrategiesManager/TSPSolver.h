#pragma once

#include <cstddef>

namespace Scheduler
{
	class Schedule;

	class TSPSolver
	{
	public:
		virtual ~TSPSolver(){}

		virtual void optimize(Schedule* schedule) const = 0;
		virtual void optimize(Schedule* schedule, size_t run_index) const = 0;
	};
}