#pragma once
#include <boost/optional.hpp>

namespace Scheduler
{
	template <typename T>
	using Optional = boost::optional<T>;

	template <typename T>
	bool operator==(const Optional<const T&>& lhs, const T& rhs)
	{
		return lhs.get( ) == rhs;
	}

	static const boost::none_t& None = boost::none;
}