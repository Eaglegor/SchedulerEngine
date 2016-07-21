#include "TimeWindowsConstraint.h"

namespace Scheduler
{
	TimeWindowsConstraint::TimeWindowsConstraint(const std::vector<TimeWindow>& time_windows):
		time_windows(time_windows)
	{
	}

	TimeWindowsConstraint::TimeWindowsConstraint(const std::initializer_list<TimeWindow>& time_windows):
		time_windows(time_windows)
	{
	}

	void TimeWindowsConstraint::addTimeWindow(const TimeWindow & time_window)
	{
		time_windows.push_back(time_window);
	}

	const std::vector<TimeWindow>& TimeWindowsConstraint::get() const
	{
		return time_windows;
	}
}