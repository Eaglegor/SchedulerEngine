#include "Distance.h"

namespace Scheduler
{
	Distance::Distance( )
	    : value(0)
	{
	}

	Distance::Distance(Distance::value_type value)
	    : value(value)
	{
	}

	Distance::Distance(const Distance& rhs)
	    : value(rhs.value)
	{
	}

	Distance::value_type Distance::getValue( ) const
	{
		return value;
	}

	void Distance::setValue(Distance::value_type value)
	{
		this->value = value;
	}

	Distance& Distance::operator=(const Distance& rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Distance Distance::operator+(const Distance& rhs) const
	{
		return Distance(value + rhs.value);
	}

	Distance Distance::operator-(const Distance& rhs) const
	{
		return Distance(value - rhs.value);
	}

	Distance& Distance::operator+=(const Distance& rhs)
	{
		this->value += rhs.value;
		return *this;
	}

	Distance& Distance::operator-=(const Distance& rhs)
	{
		this->value -= rhs.value;
		return *this;
	}

	bool Distance::operator==(const Distance& rhs) const
	{
		return fabs(this->value - rhs.value) <= FLOAT_EPSILON;
	}

	bool Distance::operator!=(const Distance& rhs) const
	{
		return !(*this == rhs);
	}

	bool Distance::operator>(const Distance& rhs) const
	{
		return this->value > rhs.value;
	}

	bool Distance::operator<(const Distance& rhs) const
	{
		return this->value < rhs.value;
	}

	bool Distance::operator>=(const Distance& rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Distance::operator<=(const Distance& rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	Distance Distance::operator-( ) const
	{
		return Distance(-value);
	}

	Distance Distance::operator*(Distance::value_type multiplier) const
	{
		return Distance(this->value * multiplier);
	}

	Distance& Distance::operator*=(Distance::value_type multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Distance Distance::operator/(Distance::value_type divider) const
	{
		return Distance(this->value / divider);
	}

	Distance& Distance::operator/=(Distance::value_type divider)
	{
		this->value /= divider;
		return *this;
	}
}