#pragma once

#include <cmath>
#include <Engine/Math/FloatEpsilon.h>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Cost
	{
	public:
		using value_type = float;
		
		Cost();
		explicit Cost(float value);
		Cost(const Cost &cost);

		float getValue() const;
		void setValue(float value);

		bool operator==(const Cost &rhs) const;
		bool operator!=(const Cost &rhs) const;

		bool operator>(const Cost &rhs) const;
		bool operator<(const Cost &rhs) const;

		bool operator>=(const Cost &rhs) const;
		bool operator<=(const Cost &rhs) const;

		Cost operator+(const Cost &rhs) const;
		Cost operator-(const Cost &rhs) const;
		Cost operator*(float multiplier) const;
		Cost operator/(float divider) const;
		Cost& operator*=(float multiplier);
		Cost& operator/=(float divider);

		Cost& operator+=(const Cost &rhs);
		Cost& operator-=(const Cost &rhs);

		Cost& operator=(const Cost &rhs);

		Cost operator-() const;

	private:
		float value;
	};
}