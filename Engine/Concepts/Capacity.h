#pragma once

#include <Engine/Math/FloatEpsilon.h>
#include <algorithm>
#include <cmath>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Capacity
	{
	public:
		Capacity( );
		Capacity(float d1, float d2 = 0, float d3 = 0, float d4 = 0);
		Capacity(const Capacity& rhs);

		const float& operator[](size_t index) const;
		float& operator[](size_t index);

		float getValue(size_t dimension_index) const;
		void setValue(size_t dimension_index, float value);
		void setValues(float d1, float d2 = 0, float d3 = 0, float d4 = 0);

		bool operator==(const Capacity& rhs) const;
		bool operator!=(const Capacity& rhs) const;

		bool operator<(const Capacity& rhs) const;
		bool operator>(const Capacity& rhs) const;

		bool operator<=(const Capacity& rhs) const;
		bool operator>=(const Capacity& rhs) const;

		Capacity operator+(const Capacity& rhs) const;
		Capacity operator-(const Capacity& rhs) const;

		Capacity& operator+=(const Capacity& rhs);
		Capacity& operator-=(const Capacity& rhs);

		Capacity& operator=(const Capacity& rhs);

		Capacity operator-( ) const;

	private:
		float capacity[4];
	};
}