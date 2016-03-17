#pragma once

#include "Duration.h"
#include <limits>
#include <ctime>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT TimePoint
	{
	public:
		TimePoint();
		explicit TimePoint(std::time_t value);
		TimePoint(const TimePoint &rhs);

		std::time_t getValue() const;
		void setValue(std::time_t value);

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
		std::time_t value;
	};
 }