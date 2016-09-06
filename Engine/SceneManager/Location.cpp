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
	
	const char* Location::getName() const
	{
		return name.c_str();
	}
	
	const Site& Location::getSite() const
	{
		return site;
	}
	
	void Location::setName(const char* name)
	{
		this->name = name;
	}
	
	bool Location::operator==(const Location& rhs) const
	{
		return site == rhs.site;
	}
	
	bool Location::operator!=(const Location& rhs) const
	{
		return !(*this == rhs);
	}
}