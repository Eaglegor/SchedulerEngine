#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Stop;
	class TimeWindow;

	class SCHEDULERENGINE_EXPORT TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop* stop) = 0;
	};
}