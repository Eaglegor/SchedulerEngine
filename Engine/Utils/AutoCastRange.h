#pragma once


#include "DerivedTypeCaster.h"
#include <boost/range/adaptor/transformed.hpp>

namespace Scheduler
{
	/**
	 * @ingroup utils
	 * 
	 * @brief Creates an autocast range, i.e. container adapter that casts all elements to T on retrieving
	 * 
	 * @tparam T The type to cast the container elements to
	 * @tparam BaseContainer The container type to create the adapter for
	 * 
	 * @return The autocast range for the passed container
	 */
	template <typename T, typename BaseContainer>
	auto make_autocast_range(BaseContainer& container) -> decltype(container | boost::adaptors::transformed(DerivedTypeCaster<typename BaseContainer::value_type, T>( )))
	{
		return container | boost::adaptors::transformed(DerivedTypeCaster<typename BaseContainer::value_type, T>( ));
	}
}