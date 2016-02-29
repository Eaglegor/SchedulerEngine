#pragma once

#include <Engine/Concepts/Basic/TimeWindow.h>

namespace Scheduler
{
	TimeWindow make_time_window(long start_time, long end_time)
	{
		return TimeWindow(TimePoint(start_time), TimePoint(end_time));
	}
}