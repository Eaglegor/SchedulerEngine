#include "TimePoint.h"

namespace Scheduler
{

	TimePoint::TimePoint( )
	    : value(0)
	{
	}

	TimePoint::TimePoint(value_type value)
	    : value(value)
	{
	}

	TimePoint::TimePoint(const TimePoint& rhs)
	    : value(rhs.value)
	{
	}

	TimePoint& TimePoint::operator=(const TimePoint& rhs)
	{
		this->value = rhs.value;
		return *this;
	}

	value_type TimePoint::getValue( ) const
	{
		return this->value;
	}

	void TimePoint::setValue(value_type value)
	{
		this->value = value;
	}

	TimePoint& TimePoint::operator+=(const Duration& duration)
	{
		this->value += duration.getValue( );
		return *this;
	}

	TimePoint& TimePoint::operator-=(const Duration& duration)
	{
		this->value -= duration.getValue( );
		return *this;
	}

	Duration TimePoint::operator-(const TimePoint& rhs) const
	{
		return Duration(value - rhs.value);
	}

	TimePoint TimePoint::operator+(const Duration& duration) const
	{
		return TimePoint(value + duration.getValue( ));
	}

	TimePoint TimePoint::operator-(const Duration& duration) const
	{
		return TimePoint(value - duration.getValue( ));
	}

	bool TimePoint::operator<(const TimePoint& rhs) const
	{
		return value < rhs.value;
	}

	bool TimePoint::operator<=(const TimePoint& rhs) const
	{
		return *this < rhs || *this == rhs;
	}

	bool TimePoint::operator>(const TimePoint& rhs) const
	{
		return value > rhs.value;
	}

	bool TimePoint::operator>=(const TimePoint& rhs) const
	{
		return *this > rhs || *this == rhs;
	}

	bool TimePoint::operator==(const TimePoint& rhs) const
	{
		return value == rhs.value;
	}

	bool TimePoint::operator!=(const TimePoint& rhs) const
	{
		return !(*this == rhs);
	}

	TimePoint TimePoint::max( )
	{
		return TimePoint(std::numeric_limits<value_type>::max( ));
	}

	TimePoint TimePoint::min( )
	{
		return TimePoint(0);
	}
}