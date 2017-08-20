#include "Capacity.h"

namespace Scheduler
{
	Capacity::Capacity( )
	{
		for(size_t i = 0; i < 4; ++i)
		{
			capacity[i] = 0.0f;
		}
	}

	Capacity::Capacity(float d1, float d2, float d3, float d4)
	{
		capacity[0] = d1;
		capacity[1] = d2;
		capacity[2] = d3;
		capacity[3] = d4;
	}

	Capacity::Capacity(const Capacity& rhs)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			capacity[i] = rhs.capacity[i];
		}
	}

	const float& Capacity::operator[](size_t index) const
	{
		return capacity[index];
	}

	float& Capacity::operator[](size_t index)
	{
		return capacity[index];
	}

	float Capacity::getValue(size_t dimension_index) const
	{
		return capacity[dimension_index];
	}

	void Capacity::setValue(size_t dimension_index, float value)
	{
		capacity[dimension_index] = value;
	}

	void Capacity::setValues(float d1, float d2, float d3, float d4)
	{
		capacity[0] = d1;
		capacity[1] = d2;
		capacity[2] = d3;
		capacity[3] = d4;
	}

	bool Capacity::operator==(const Capacity& rhs) const
	{
		for(size_t i = 0; i < 4; ++i)
		{
			if(fabs(getValue(i) - rhs.getValue(i)) > FLOAT_EPSILON)
			{
				return false;
			}
		}
		return true;
	}

	bool Capacity::operator<(const Capacity& rhs) const
	{
		for(size_t i = 0; i < 4; ++i)
		{
			if(capacity[i] > rhs.capacity[i] + FLOAT_EPSILON) return false;
		}
		return true;
	}

	bool Capacity::operator>(const Capacity& rhs) const
	{
		for(size_t i = 0; i < 4; ++i)
		{
			if(capacity[i] < rhs.capacity[i] - FLOAT_EPSILON) return false;
		}
		return true;
	}

	Capacity& Capacity::operator+=(const Capacity& rhs)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			capacity[i] += rhs.capacity[i];
		}
		return *this;
	}

	Capacity& Capacity::operator-=(const Capacity& rhs)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			capacity[i] -= rhs.capacity[i];
		}
		return *this;
	}

	Capacity& Capacity::operator=(const Capacity& rhs)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			capacity[i] = rhs.capacity[i];
		}
		return *this;
	}

	bool Capacity::operator!=(const Capacity& rhs) const
	{
		return !(*this == rhs);
	}

	bool Capacity::operator<=(const Capacity& rhs) const
	{
		return *this == rhs || *this < rhs;
	}

	bool Capacity::operator>=(const Capacity& rhs) const
	{
		return *this == rhs || *this > rhs;
	}

	Capacity Capacity::operator+(const Capacity& rhs) const
	{
		Capacity result(*this);
		result += rhs;
		return result;
	}

	Capacity Capacity::operator-(const Capacity& rhs) const
	{
		Capacity result(*this);
		result -= rhs;
		return result;
	}

	Capacity Capacity::operator-( ) const
	{
		Capacity result(*this);
		for(int i = 0; i < 4; ++i)
		{
			result.setValue(i, -result.getValue(i));
		}
		return result;
	}
}
