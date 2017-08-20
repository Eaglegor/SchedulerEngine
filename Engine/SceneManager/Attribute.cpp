#include "Attribute.h"

namespace Scheduler
{
	Attribute::Attribute(std::size_t id, const String& name)
	    : id(id),
	      name(name)
	{
	}

	std::size_t Attribute::getId( ) const
	{
		return id;
	}

	const String& Attribute::getName( ) const
	{
		return name;
	}

	bool Attribute::operator==(const Attribute& rhs) const
	{
		return id == rhs.id && this == &rhs;
	}

	bool Attribute::operator!=(const Attribute& rhs) const
	{
		return !(*this == rhs);
	}
}
