#pragma once

#include <cmath>
#include <Engine/Math/FloatEpsilon.h>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Coordinate
	{
	public:
		Coordinate();
		explicit Coordinate(float value);
		Coordinate(const Coordinate &rhs);

		float getValue() const;
		void setValue(float value);

		bool operator==(const Coordinate &rhs) const;
		bool operator!=(const Coordinate &rhs) const;

		bool operator<(const Coordinate &rhs) const;
		bool operator>(const Coordinate &rhs) const;

		bool operator<=(const Coordinate &rhs) const;
		bool operator>=(const Coordinate &rhs) const;

		Coordinate operator+(const Coordinate &rhs) const;
		Coordinate operator-() const;
		Coordinate operator*(float multiplier) const;
		Coordinate operator/(float divider) const;

		Coordinate& operator+=(const Coordinate &rhs);
		Coordinate& operator-=(const Coordinate &rhs);
		Coordinate& operator*=(float multiplier);
		Coordinate& operator/=(float divider);

		Coordinate& operator=(const Coordinate &rhs);

		Coordinate operator-(const Coordinate &rhs) const;

	private:
		float value;
	};
}