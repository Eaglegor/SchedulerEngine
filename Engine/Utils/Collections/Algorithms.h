#pragma once

#include <algorithm>
#include <functional>

namespace util
{
	/**
	 * @brief Checks if the collection contains the specified key
	 * 
	 * @param collection The collection to search in
	 * @param key The key to search for
	 * 
	 * @return True if the collection contains the passed key
	 */
	template <typename T>
	bool contains_key(const T& collection, const typename T::key_type& key)
	{
		return collection.find(key) != collection.end( );
	}

	/**
	 * @brief Checks if the collection contains the specified value
	 * 
	 * @param collection The collection to search in
	 * @param value The value to search for
	 * 
	 * @return True if the collection contains the passed value
	 */
	template <typename T>
	bool contains(const T& collection, const typename T::value_type& value)
	{
		return std::find(collection.begin( ), collection.end( ), value) != collection.end( );
	}

	/**
	 * @brief Checks if the collection contains the value matching the passed predicate
	 * 
	 * @param collection The collection to search in
	 * @param predicate The predicate to test the values against
	 * 
	 * @return True if the collection contains the value matching the predicate
	 */
	template <typename T, typename UnaryPredicate>
	bool contains_if(const T& collection, UnaryPredicate&& predicate)
	{
		return std::find_if(collection.begin( ), collection.end( ), std::forward<UnaryPredicate>(predicate)) != collection.end( );
	}

	/**
	 * @brief Retrieves the iterator to the element in the collection
	 * 
	 * @param collection The collection to search the value in
	 * @param value The value to search for
	 * 
	 * @return The iterator to the found element or collection.end() if the element wasn't found
	 */
	template <typename T>
	typename T::const_iterator find_iterator(T& collection, const typename T::value_type& value)
	{
		return std::find(collection.begin( ), collection.end( ), value);
	}
}