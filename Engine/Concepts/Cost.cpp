#include "Cost.h"

namespace Scheduler
{
	Cost::Cost( )
	    : value(0)
	{
	}

	Cost::Cost(value_type value)
	    : value(value)
	{
	}

	Cost::Cost(const Cost& cost)
	    : value(cost.value)
	{
	}

	value_type Cost::getValue( ) const
	{
		return value;
	}

	void Cost::setValue(value_type value)
	{
		this->value = value;
	}

	Cost& Cost::operator=(const Cost& rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Cost& Cost::operator=(const value_type& rhs)
	{
		this->value = rhs;
		return *this;
	}

	Cost Cost::operator+(const Cost& rhs) const
	{
		return Cost(value + rhs.value);
	}

	Cost Cost::operator-(const Cost& rhs) const
	{
		return Cost(value - rhs.value);
	}

	Cost& Cost::operator+=(const Cost& rhs)
	{
		this->value += rhs.value;
		return *this;
	}

	Cost& Cost::operator-=(const Cost& rhs)
	{
		this->value -= rhs.value;
		return *this;
	}

	bool Cost::operator==(const Cost& rhs) const
	{
		return fabs(this->value - rhs.value) <= FLOAT_EPSILON;
	}

	bool Cost::operator!=(const Cost& rhs) const
	{
		return !(*this == rhs);
	}

	bool Cost::operator>(const Cost& rhs) const
	{
		return this->value > rhs.value;
	}

	bool Cost::operator<(const Cost& rhs) const
	{
		return this->value < rhs.value;
	}

	bool Cost::operator>=(const Cost& rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Cost::operator<=(const Cost& rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	Cost Cost::operator-( ) const
	{
		return Cost(-value);
	}

	Cost Cost::operator*(value_type multiplier) const
	{
		return Cost(this->value * multiplier);
	}

	Cost& Cost::operator*=(value_type multiplier)
	{
		this->value *= multiplier;
		return *this;
	}
	
	Cost Cost::operator/(value_type divider) const
	{
		return Cost(this->value / divider);
	}

	Cost& Cost::operator/=(value_type divider)
	{
		this->value /= divider;
		return *this;
	}
}