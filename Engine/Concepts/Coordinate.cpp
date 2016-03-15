#include "Coordinate.h"

namespace Scheduler
{

	Coordinate::Coordinate() : value(0)
	{ }

	Coordinate::Coordinate(float value) : value(value)
	{ }

	Coordinate::Coordinate(const Coordinate &rhs) : value(rhs.value)
	{ }

	float Coordinate::getValue() const
	{
		return value;
	}

	void Coordinate::setValue(float value)
	{
		this->value = value;
	}

	Coordinate& Coordinate::operator=(const Coordinate &rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	bool Coordinate::operator==(const Coordinate &rhs) const
	{
		return fabs(value - rhs.value) < FLOAT_EPSILON;
	}

	bool Coordinate::operator!=(const Coordinate &rhs) const
	{
		return !(*this == rhs);
	}

	bool Coordinate::operator<(const Coordinate &rhs) const
	{
		return value < rhs.value - FLOAT_EPSILON;
	}

	bool Coordinate::operator<=(const Coordinate &rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool Coordinate::operator>(const Coordinate &rhs) const
	{
		return value > rhs.value + FLOAT_EPSILON;
	}

	bool Coordinate::operator>=(const Coordinate &rhs) const
	{
		return *this > rhs || *this == rhs;
	}

	Coordinate Coordinate::operator+(const Coordinate &rhs) const
	{
		return Coordinate(value + rhs.value);
	}

	Coordinate& Coordinate::operator+=(const Coordinate &rhs)
	{
		this->value += rhs.value;
		return *this;
	}

	Coordinate Coordinate::operator-(const Coordinate &rhs) const
	{
		return Coordinate(value - rhs.value);
	}

	Coordinate& Coordinate::operator-=(const Coordinate &rhs)
	{
		this->value -= rhs.value;
		return *this;
	}

	Coordinate Coordinate::operator-() const
	{
		return Coordinate(-value);
	}

	Coordinate Coordinate::operator*(float multiplier) const
	{
		return Coordinate(this->value * multiplier);
	}

	Coordinate& Coordinate::operator*=(float multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Coordinate Coordinate::operator/(float divider) const
	{
		return Coordinate(this->value / divider);
	}

	Coordinate& Coordinate::operator/=(float divider)
	{
		this->value /= divider;
		return *this;
	}
}