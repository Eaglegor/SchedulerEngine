#pragma once

#include <Engine/Utils/String.h>

#include <cstddef>
#include <string>

namespace Scheduler
{
	class Location;

	class Depot
	{
	public:
		Depot(std::size_t id, const Location& location);

		std::size_t getId( ) const;
		const Location& getLocation( ) const;

		const String& getName( ) const;
		void setName(const String& name);

		bool operator==(const Depot& rhs) const;
		bool operator!=(const Depot& rhs) const;

	private:
		std::size_t id;
		String name;
		const Location& location;
	};
}