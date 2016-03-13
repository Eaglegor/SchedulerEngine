#pragma once

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Stop;
	class TimeWindow;

	/// Selects one of multiple stop's time windows using some rule
	class SCHEDULERENGINE_EXPORT TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop* stop) = 0;
	};
}