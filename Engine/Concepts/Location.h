#pragma once

#include <cmath>
#include "Coordinate.h"

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Location
	{
	public:
		Location();
		Location(const Location &rhs);
		Location(Coordinate latitude, Coordinate longitude);

		Coordinate getLatitude() const;
		Coordinate getLongitude() const;

		void setLatitude(const Coordinate &latitude);
		void setLongitude(const Coordinate &longitude);

		bool operator==(const Location &rhs) const;
		bool operator!=(const Location &rhs) const;

		Location& operator=(const Location &rhs);
		
	private:
		Coordinate latitude;
		Coordinate longitude;
	};
}