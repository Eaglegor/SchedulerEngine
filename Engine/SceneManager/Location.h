#pragma once

#include <Engine/Concepts/Site.h>
#include <Engine/Utils/String.h>

#include <cstddef>
#include <string>

namespace Scheduler
{
	class Location
	{
	public:
		Location(std::size_t id, const Site& site);

		std::size_t getId( ) const;
		const Site& getSite( ) const;

		const String& getName( ) const;
		void setName(const String& name);

		bool operator==(const Location& rhs) const;
		bool operator!=(const Location& rhs) const;

	private:
		std::size_t id;
		String name;
		Site site;
	};
}