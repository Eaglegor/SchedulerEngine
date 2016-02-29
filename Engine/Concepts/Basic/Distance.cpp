#include "Distance.h"

namespace Scheduler
{
	Distance::Distance() : value(0)
	{ }

	Distance::Distance(float value) : value(value)
	{ }

	Distance::Distance(const Distance &rhs) : value(rhs.value)
	{ }

	float Distance::getValue() const
	{
		return value;
	}

	void Distance::setValue(float value)
	{
		this->value = value;
	}

	Distance& Distance::operator=(const Distance &rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Distance Distance::operator+(const Distance &rhs) const
	{
		return Distance(value + rhs.value);
	}

	Distance Distance::operator-(const Distance &rhs) const
	{
		return Distance(value - rhs.value);
	}

	Distance& Distance::operator+=(const Distance &rhs)
	{
		this->value += rhs.value;
		return *this;
	}

	Distance& Distance::operator-=(const Distance &rhs)
	{
		this->value -= rhs.value;
		return *this;
	}

	bool Distance::operator==(const Distance &rhs) const
	{
		return fabs(this->value - rhs.value) <= FLOAT_EPSILON;
	}

	bool Distance::operator!=(const Distance &rhs) const
	{
		return !(*this == rhs);
	}

	bool Distance::operator>(const Distance &rhs) const
	{
		return this->value > rhs.value;
	}

	bool Distance::operator<(const Distance &rhs) const
	{
		return this->value < rhs.value;
	}

	bool Distance::operator>=(const Distance &rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Distance::operator<=(const Distance &rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	Distance Distance::operator-() const
	{
		return Distance(-value);
	}

	Distance Distance::operator*(float multiplier) const
	{
		return Distance(this->value * multiplier);
	}

	Distance& Distance::operator*=(float multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Distance Distance::operator/(float divider) const
	{
		return Distance(this->value / divider);
	}

	Distance& Distance::operator/=(float divider)
	{
		this->value /= divider;
		return *this;
	}
}