#pragma once

#include <cstddef>
#include <Engine/Math/FixedPointMath.h>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Coordinate
	{
	public:
		static const size_t PRECISION = 6;

		Coordinate();
		explicit Coordinate(FixedPointType value);
		Coordinate(const Coordinate &rhs);

		FixedPointType getValue() const;
		void setValue(FixedPointType value);

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

		static Coordinate createFromFloat(float value);

	private:
		FixedPointType value;
	};
}