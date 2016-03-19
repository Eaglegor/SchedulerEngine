#pragma once


namespace Scheduler
{
	class Stop;
	class TimeWindow;

	/// Selects one of multiple stop's time windows using some rule
	class TimeWindowSelector
	{
	public:
		virtual ~TimeWindowSelector(){}
		virtual TimeWindow selectTimeWindow(Stop* stop) = 0;

		virtual const char* getName() const = 0;
	};
}