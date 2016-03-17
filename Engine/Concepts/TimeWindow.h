#pragma once

#include <limits>
#include "TimePoint.h"

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT TimeWindow
	{
	public:
		TimeWindow();
		TimeWindow(const TimeWindow &rhs);
		TimeWindow(const TimePoint &start_time, const TimePoint &end_time);

		const TimePoint& getStartTime() const;
		const TimePoint& getEndTime() const;

		void setStartTime(const TimePoint &start_time);
		void setEndTime(const TimePoint &end_time);

		Duration getDuration() const;

		bool operator==(const TimeWindow &rhs) const;
		bool operator!=(const TimeWindow &rhs) const;

		TimeWindow& operator=(const TimeWindow &rhs);

	private:
		TimePoint start_time;
		TimePoint end_time;
	};
}