#pragma once

#include <cmath>
#include <Math/FloatEpsilon.h>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT Cost
	{
	public:
		Cost();
		explicit Cost(float value);
		Cost(const Cost &cost);

		float getValue() const;
		void setValue(float value);

		bool operator==(const Cost &rhs) const;
		bool operator!=(const Cost &rhs) const;

		bool operator>(const Cost &rhs) const;
		bool operator<(const Cost &rhs) const;

		bool operator>=(const Cost &rhs) const;
		bool operator<=(const Cost &rhs) const;

		Cost operator+(const Cost &rhs) const;
		Cost operator-(const Cost &rhs) const;
		Cost operator*(float multiplier) const;
		Cost operator/(float divider) const;
		Cost& operator*=(float multiplier);
		Cost& operator/=(float divider);

		Cost& operator+=(const Cost &rhs);
		Cost& operator-=(const Cost &rhs);

		Cost& operator=(const Cost &rhs);

		Cost operator-() const;

	private:
		float value;
	};
}