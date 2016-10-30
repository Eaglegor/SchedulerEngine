#pragma once


#include "DerivedTypeCaster.h"
#include <boost/range/adaptor/transformed.hpp>

namespace Scheduler
{
	template<typename T, typename BaseContainer>
	auto make_autocast_range(BaseContainer &container) -> decltype(container | boost::adaptors::transformed(DerivedTypeCaster<typename BaseContainer::value_type, T>()))
	{
		return container | boost::adaptors::transformed(DerivedTypeCaster<typename BaseContainer::value_type, T>());
	}
}