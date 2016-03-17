#pragma once

#include <cstddef>

namespace Scheduler
{
	class Schedule;

	class TSPSolver
	{
	public:
		virtual ~TSPSolver(){}

		virtual void optimize(Schedule* schedule) = 0;
		virtual void optimize(Schedule* schedule, size_t run_index) = 0;
	};
}