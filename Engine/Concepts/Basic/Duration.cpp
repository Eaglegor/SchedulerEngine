#include "Duration.h"

namespace Scheduler
{

	Duration::Duration() : value(0)
	{ }

	Duration::Duration(long value) : value(value)
	{ }

	Duration::Duration(const Duration &rhs) : value(rhs.value)
	{ }

	long Duration::getValue() const
	{
		return value;
	}

	void Duration::setValue(long value)
	{
		this->value = value;
	}

	Duration& Duration::operator=(const Duration &rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	Duration Duration::operator+(const Duration &rhs) const
	{
		return Duration(value + rhs.value);
	}

	Duration Duration::operator-(const Duration &rhs) const
	{
		return Duration(value - rhs.value);
	}

	Duration& Duration::operator+=(const Duration &rhs)
	{
		value += rhs.value;
		return *this;
	}

	Duration& Duration::operator-=(const Duration &rhs)
	{
		value -= rhs.value;
		return *this;
	}

	bool Duration::operator<(const Duration &rhs) const
	{
		return value < rhs.value;
	}

	bool Duration::operator<=(const Duration &rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	bool Duration::operator>(const Duration &rhs) const
	{
		return value > rhs.value;
	}

	bool Duration::operator>=(const Duration &rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	bool Duration::operator==(const Duration &rhs) const
	{
		return value == rhs.value;
	}

	bool Duration::operator!=(const Duration &rhs) const
	{
		return !(*this == rhs);
	}

	Duration Duration::operator-() const
	{
		return Duration(-value);
	}

	Duration Duration::operator*(long multiplier) const
	{
		return Duration(this->value * multiplier);
	}

	Duration& Duration::operator*=(long multiplier)
	{
		this->value *= multiplier;
		return *this;
	}

	Duration Duration::operator/(long divider) const
	{
		return Duration(this->value / divider);
	}

	Duration& Duration::operator/=(long divider)
	{
		this->value /= divider;
		return *this;
	}

	Duration Duration::max()
	{
		return Duration(std::numeric_limits<long>::max());
	}
}