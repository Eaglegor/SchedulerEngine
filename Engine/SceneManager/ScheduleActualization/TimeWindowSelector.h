#pragma once

namespace Scheduler
{
	class Stop;
	class TimeWindow;

	class TimeWindowSelector
	{
	public:
		virtual TimeWindow selectTimeWindow(Stop* stop) = 0;
	};
}