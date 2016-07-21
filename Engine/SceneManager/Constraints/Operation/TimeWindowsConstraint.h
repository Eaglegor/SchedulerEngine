#pragma once

#include <vector>
#include <Engine/Concepts/TimeWindow.h>

namespace Scheduler
{
	class TimeWindowsConstraint
	{
	public:
		TimeWindowsConstraint() = default;
		explicit TimeWindowsConstraint(const std::vector<TimeWindow>& time_windows);
		explicit TimeWindowsConstraint(const std::initializer_list<TimeWindow>& time_windows);

		void addTimeWindow(const TimeWindow& time_window);

		const std::vector<TimeWindow>& get() const;

	private:
		std::vector<TimeWindow> time_windows;
	};
}