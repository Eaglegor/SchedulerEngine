#pragma once

#include <cmath>
#include <algorithm>
#include <Math/FloatEpsilon.h>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT Capacity
	{
	public:
		Capacity();
		Capacity(float d1, float d2, float d3, float d4);
		Capacity(const Capacity &rhs);

		const float& operator[](size_t index) const;
		float& operator[](size_t index);

		float getValue(size_t dimension_index) const;
		void setValue(size_t dimension_index, float value);
		void setValues(float d1, float d2, float d3, float d4);

		bool operator==(const Capacity &rhs) const;
		bool operator!=(const Capacity &rhs) const;

		bool operator<(const Capacity &rhs) const;
		bool operator>(const Capacity &rhs) const;

		bool operator<=(const Capacity &rhs) const;
		bool operator>=(const Capacity &rhs) const;

		Capacity operator+(const Capacity &rhs) const;
		Capacity operator-(const Capacity &rhs) const;

		Capacity& operator+=(const Capacity &rhs);
		Capacity& operator-=(const Capacity &rhs);

		Capacity& operator=(const Capacity &rhs);

		Capacity operator-() const;

	private:
		float capacity[4];
	};
}