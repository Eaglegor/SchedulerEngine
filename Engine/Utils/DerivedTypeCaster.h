#pragma once

namespace Scheduler
{
	template <typename Base, typename Derived>
	struct DerivedTypeCaster
	{
		Derived& operator( )(Base& value) const
		{
			return static_cast<Derived&>(value);
		};

		const Derived& operator( )(const Base& value) const
		{
			return static_cast<const Derived&>(value);
		};

		Derived* operator( )(Base* value) const
		{
			return static_cast<Derived*>(value);
		};

		const Derived* operator( )(const Base* value) const
		{
			return static_cast<const Derived*>(value);
		};
	};
}