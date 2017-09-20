#pragma once
#include <boost/optional.hpp>

namespace Scheduler
{
	/** 
	 * @ingroup utils
	 * @{
	 */

	/*
	 * @brief Optional class to use inside the scheduler
	 * 
	 * @details boost::optional is used as the implementation
	 */ 
	template <typename T>
	using Optional = boost::optional<T>;

	template <typename T>
	bool operator==(const Optional<const T&>& lhs, const T& rhs)
	{
		return lhs.get( ) == rhs;
	}

	static const boost::none_t& None = boost::none;

	/**
	 * @}
	 */
}