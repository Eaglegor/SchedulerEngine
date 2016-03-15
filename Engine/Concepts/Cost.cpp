#include "Cost.h"

namespace Scheduler
{
	Cost::Cost() : value(0)
	{ }

	Cost::Cost(float value) : value(value)
	{ }

	Cost::Cost(const Cost &cost) : value(cost.value)
	{ }

	float Cost::getValue() const
	{
		return value;
	}

	void Cost::setValue(float value)
	{
		this->value = value;
	}

	Cost& Cost::operator=(const Cost &rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Cost Cost::operator+(const Cost &rhs) const
	{
		return Cost(value + rhs.value);
	}

	Cost Cost::operator-(const Cost &rhs) const
	{
		return Cost(value - rhs.value);
	}

	Cost& Cost::operator+=(const Cost &rhs)
	{
		this->value += rhs.value;
		return *this;
	}

	Cost& Cost::operator-=(const Cost &rhs)
	{
		this->value -= rhs.value;
		return *this;
	}

	bool Cost::operator==(const Cost &rhs) const
	{
		return fabs(this->value - rhs.value) <= FLOAT_EPSILON;
	}

	bool Cost::operator!=(const Cost &rhs) const
	{
		return !(*this == rhs);
	}

	bool Cost::operator>(const Cost &rhs) const
	{
		return this->value > rhs.value;
	}

	bool Cost::operator<(const Cost &rhs) const
	{
		return this->value < rhs.value;
	}

	bool Cost::operator>=(const Cost &rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Cost::operator<=(const Cost &rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	Cost Cost::operator-() const
	{
		return Cost(-value);
	}

	Cost Cost::operator*(float multiplier) const
	{
		return Cost(this->value * multiplier);
	}

	Cost& Cost::operator*=(float multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Cost Cost::operator/(float divider) const
	{
		return Cost(this->value / divider);
	}

	Cost& Cost::operator/=(float divider)
	{
		this->value /= divider;
		return *this;
	}
}