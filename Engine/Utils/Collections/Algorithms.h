#pragma once

#include <algorithm>
#include <functional>

namespace util
{
	template <typename T>
	bool contains_key(const T& collection, const typename T::key_type& key)
	{
		return collection.find(key) != collection.end( );
	}

	template <typename T>
	bool contains(const T& collection, const typename T::value_type& value)
	{
		return std::find(collection.begin( ), collection.end( ), value) != collection.end( );
	}

	template <typename T, typename UnaryPredicate>
	bool contains_if(const T& collection, UnaryPredicate&& predicate)
	{
		return std::find_if(collection.begin( ), collection.end( ), std::forward<UnaryPredicate>(predicate)) != collection.end( );
	}

	template <typename T>
	typename T::const_iterator find_iterator(T& collection, const typename T::value_type& value)
	{
		return std::find(collection.begin( ), collection.end( ), value);
	}
}