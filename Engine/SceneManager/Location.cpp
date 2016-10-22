#include "Location.h"

namespace Scheduler
{
	Location::Location(std::size_t id, const Site& site):
	id(id),
	site(site)
	{}
	
	std::size_t Location::getId() const
	{
		return id;
	}
	
	const String& Location::getName() const
	{
		return name;
	}
	
	const Site& Location::getSite() const
	{
		return site;
	}
	
	void Location::setName(const String& name)
	{
		this->name = name;
	}
	
	bool Location::operator==(const Location& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}
	
	bool Location::operator!=(const Location& rhs) const
	{
		return !(*this == rhs);
	}
}