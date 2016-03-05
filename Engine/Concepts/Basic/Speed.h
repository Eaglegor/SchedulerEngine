#pragma once

#include <Engine/Concepts/Basic/Distance.h>
#include <Math/FloatEpsilon.h>

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT Speed
	{
	public:
		Speed();
		explicit Speed(const Distance &value);
		Speed(const Speed &rhs);

		const Distance& getValue() const;
		void setValue(const Distance &value);

		bool operator==(const Speed &rhs) const;
		bool operator!=(const Speed &rhs) const;

		bool operator>(const Speed &rhs) const;
		bool operator<(const Speed &rhs) const;

		bool operator>=(const Speed &rhs) const;
		bool operator<=(const Speed &rhs) const;

		Speed operator+(const Speed &rhs) const;
		Speed operator-(const Speed &rhs) const;
		Speed operator*(float multiplier) const;
		Speed operator/(float divider) const;

		Speed& operator+=(const Speed &rhs);
		Speed& operator-=(const Speed &rhs);
		Speed& operator*=(float multiplier);
		Speed& operator/=(float divider);

		Speed& operator=(const Speed &rhs);

		Speed operator-() const;

	private:
		Distance value;
	};
}