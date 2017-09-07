#include "Duration.h"

namespace Scheduler
{

	Duration::Duration( )
	    : value(0)
	{
	}

	Duration::Duration(Duration::value_type value)
	    : value(value)
	{
	}

	Duration::Duration(const Duration& rhs)
	    : value(rhs.value)
	{
	}

	Duration::value_type Duration::getValue( ) const
	{
		return value;
	}

	void Duration::setValue(Duration::value_type value)
	{
		this->value = value;
	}

	Duration& Duration::operator=(const Duration& rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Duration Duration::operator+(const Duration& rhs) const
	{
		return Duration(value + rhs.value);
	}

	Duration Duration::operator-(const Duration& rhs) const
	{
		return Duration(value - rhs.value);
	}

	Duration& Duration::operator+=(const Duration& rhs)
	{
		value += rhs.value;
		return *this;
	}

	Duration& Duration::operator-=(const Duration& rhs)
	{
		value -= rhs.value;
		return *this;
	}

	bool Duration::operator<(const Duration& rhs) const
	{
		return value < rhs.value;
	}

	bool Duration::operator<=(const Duration& rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	bool Duration::operator>(const Duration& rhs) const
	{
		return value > rhs.value;
	}

	bool Duration::operator>=(const Duration& rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Duration::operator==(const Duration& rhs) const
	{
		return value == rhs.value;
	}

	bool Duration::operator!=(const Duration& rhs) const
	{
		return !(*this == rhs);
	}

	Duration Duration::operator-( ) const
	{
		return Duration(-value);
	}

	Duration Duration::operator*(Duration::value_type multiplier) const
	{
		return Duration(this->value * multiplier);
	}

	Duration& Duration::operator*=(Duration::value_type multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Duration Duration::operator/(Duration::value_type divider) const
	{
		return Duration(this->value / divider);
	}

	Duration& Duration::operator/=(Duration::value_type divider)
	{
		this->value /= divider;
		return *this;
	}

	Duration Duration::max( )
	{
		return Duration(std::numeric_limits<Duration::value_type>::max( ));
	}
}