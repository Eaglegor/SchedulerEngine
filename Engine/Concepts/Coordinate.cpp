#include "Coordinate.h"

namespace Scheduler
{

	Coordinate::Coordinate() : value(0)
	{ }

	Coordinate::Coordinate(FixedPointType value): value(value)
	{
	}

	Coordinate::Coordinate(const Coordinate &rhs) : value(rhs.value)
	{ }

	FixedPointType Coordinate::getValue() const
	{
		return value;
	}

	void Coordinate::setValue(FixedPointType value)
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
		return value == rhs.value;
	}

	bool Coordinate::operator!=(const Coordinate &rhs) const
	{
		return !(*this == rhs);
	}

	bool Coordinate::operator<(const Coordinate &rhs) const
	{
		return value < rhs.value;
	}

	bool Coordinate::operator<=(const Coordinate &rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool Coordinate::operator>(const Coordinate &rhs) const
	{
		return value > rhs.value;
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

	Coordinate Coordinate::createFromFloat(float value)
	{
		return Coordinate(toFixedPoint<PRECISION>(value));
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

	Coordinate Coordinate::operator*(float multiplier) const
	{
		return Coordinate(static_cast<uint32_t>(std::trunc(this->value * multiplier)));
	}

	Coordinate Coordinate::operator/(float divider) const
	{
		return Coordinate(static_cast<uint32_t>(std::trunc(this->value / divider)));
	}

	Coordinate& Coordinate::operator*=(float multiplier)
	{
		this->value = static_cast<uint32_t>(std::trunc(this->value * multiplier));
		return *this;
	}

	Coordinate& Coordinate::operator/=(float divider)
	{
		this->value = static_cast<uint32_t>(std::trunc(this->value / divider));
		return *this;
	}

	Coordinate Coordinate::operator-() const
	{
		return Coordinate(-value);
	}
}