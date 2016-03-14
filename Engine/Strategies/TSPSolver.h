#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Schedule;

	class SCHEDULERENGINE_EXPORT TSPSolver
	{
	public:
		virtual ~TSPSolver(){}

		virtual void optimize(Schedule* schedule) = 0;
		virtual void optimize(Schedule* schedule, size_t run_index) = 0;
	};
}