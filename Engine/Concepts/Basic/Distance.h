#pragma once

#include <cmath>
#include <Math/FloatEpsilon.h>

namespace Scheduler
{
	class Distance
	{
	public:
		Distance();
		explicit Distance(float value);
		Distance(const Distance &rhs);

		float getValue() const;
		void setValue(float value);

		bool operator==(const Distance &rhs) const;
		bool operator!=(const Distance &rhs) const;

		bool operator>(const Distance &rhs) const;
		bool operator<(const Distance &rhs) const;

		bool operator>=(const Distance &rhs) const;
		bool operator<=(const Distance &rhs) const;

		Distance operator+(const Distance &rhs) const;
		Distance operator-(const Distance &rhs) const;
		Distance operator*(float multiplier) const;
		Distance operator/(float divider) const;

		Distance& operator+=(const Distance &rhs);
		Distance& operator-=(const Distance &rhs);
		Distance& operator*=(float multiplier);
		Distance& operator/=(float divider);

		Distance& operator=(const Distance &rhs);

		Distance operator-() const;

	private:
		float value;
	};


}