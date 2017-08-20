#pragma once

#include <functional>

namespace std
{
	template <typename T>
	struct hash<reference_wrapper<T>>
	{
		size_t operator( )(const reference_wrapper<T>& x) const
		{
			return std::hash<size_t>( )(reinterpret_cast<size_t>(&x.get( )));
		}
	};

	template <typename T>
	bool operator==(const reference_wrapper<T>& lhs, const reference_wrapper<T>& rhs)
	{
		return &lhs.get( ) == &rhs.get( );
	}

	template <typename T>
	bool operator==(const reference_wrapper<T>& lhs, const T& rhs)
	{
		return &lhs.get( ) == &rhs;
	}

	template <typename T>
	bool operator==(const reference_wrapper<const T>& lhs, const T& rhs)
	{
		return &lhs.get( ) == &rhs;
	}
}

namespace Scheduler
{
	template <typename T>
	using ReferenceWrapper = std::reference_wrapper<T>;
}