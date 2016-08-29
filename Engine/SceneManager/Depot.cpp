#include "Depot.h"

namespace Scheduler 
{
	Depot::Depot(std::size_t id, const Location& location):
	id(id),
	location(location)
	{}

	std::size_t Depot::getId() const
	{
		return id;
	}

	const Location& Depot::getLocation() const
	{
		return location;
	}
}