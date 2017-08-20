#include "Depot.h"

namespace Scheduler
{
	Depot::Depot(std::size_t id, const Location& location)
	    : id(id),
	      location(location)
	{
	}

	std::size_t Depot::getId( ) const
	{
		return id;
	}

	const Location& Depot::getLocation( ) const
	{
		return location;
	}

	const String& Depot::getName( ) const
	{
		return name;
	}

	void Depot::setName(const String& name)
	{
		this->name = name;
	}

	bool Depot::operator==(const Depot& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Depot::operator!=(const Depot& rhs) const
	{
		return !(*this == rhs);
	}
}