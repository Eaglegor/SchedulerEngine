#pragma once

#include "Duration.h"
#include <limits>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT TimePoint
	{
	public:
		TimePoint();
		explicit TimePoint(long value);
		TimePoint(const TimePoint &rhs);

		long getValue() const;
		void setValue(long value);

		bool operator==(const TimePoint &rhs) const;
		bool operator!=(const TimePoint &rhs) const;

		bool operator<(const TimePoint &rhs) const;
		bool operator>(const TimePoint &rhs) const;

		bool operator<=(const TimePoint &rhs) const;
		bool operator>=(const TimePoint &rhs) const;

		TimePoint operator+(const Duration &duration) const;
		TimePoint operator-(const Duration &duration) const;
		Duration operator-(const TimePoint &rhs) const;

		TimePoint& operator+=(const Duration &duration);
		TimePoint& operator-=(const Duration &duration);

		TimePoint& operator=(const TimePoint &rhs);

		static TimePoint max();
		static TimePoint min();

	private:
		long value;
	};
 }