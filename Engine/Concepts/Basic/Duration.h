#pragma once

#include <limits>

namespace Scheduler
{
	class Duration
	{
	public:
		Duration();
		explicit Duration(long value);
		Duration(const Duration &rhs);

		long getValue() const;
		void setValue(long value);

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
		long value;
	};
 }