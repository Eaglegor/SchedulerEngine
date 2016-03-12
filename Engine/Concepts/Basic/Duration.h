#pragma once

#include <limits>
#include <ctime>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT Duration
	{
	public:
		Duration();
		explicit Duration(std::time_t value);
		Duration(const Duration &rhs);

		std::time_t getValue() const;
		void setValue(std::time_t value);

		bool operator==(const Duration &rhs) const;
		bool operator!=(const Duration &rhs) const;

		bool operator<(const Duration &rhs) const;
		bool operator>(const Duration &rhs) const;

		bool operator<=(const Duration &rhs) const;
		bool operator>=(const Duration &rhs) const;

		Duration operator+(const Duration &rhs) const;
		Duration operator-(const Duration &rhs) const;
		Duration operator*(long multiplier) const;
		Duration operator/(long divider) const;

		Duration& operator+=(const Duration &rhs);
		Duration& operator-=(const Duration &rhs);
		Duration& operator*=(long multiplier);
		Duration& operator/=(long divider);

		Duration& operator=(const Duration &rhs);

		Duration operator-() const;

		static Duration max();

	private:
		std::time_t value;
	};
 }