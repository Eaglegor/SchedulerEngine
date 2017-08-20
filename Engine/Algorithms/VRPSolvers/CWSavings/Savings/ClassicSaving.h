#pragma once

#include <Engine/Utils/ReferenceWrapper.h>

class Order;

namespace Scheduler
{
	class ClassicSaving
	{
	public:
		ClassicSaving(const Order& i, const Order& j, float saving)
		    : i(i), j(j), saving(saving)
		{
		}
		ClassicSaving(const ClassicSaving& rhs)
		    : i(rhs.i), j(rhs.j), saving(rhs.saving)
		{
		}

		bool operator<(const ClassicSaving& rhs) const
		{
			return saving < rhs.saving;
		}
		bool operator>(const ClassicSaving& rhs) const
		{
			return saving > rhs.saving;
		}

		ClassicSaving& operator=(const ClassicSaving& rhs)
		{
			i      = rhs.i;
			j      = rhs.j;
			saving = rhs.saving;
			return *this;
		}

		ReferenceWrapper<const Order> i;
		ReferenceWrapper<const Order> j;
		float saving;
	};
}