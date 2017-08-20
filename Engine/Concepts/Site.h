#pragma once

#include "Coordinate.h"
#include <cmath>

#include <Concepts_export.h>

namespace Scheduler
{
	class CONCEPTS_EXPORT Site
	{
	public:
		Site( );
		Site(const Site& rhs);
		Site(Coordinate latitude, Coordinate longitude);

		Coordinate getLatitude( ) const;
		Coordinate getLongitude( ) const;

		void setLatitude(const Coordinate& latitude);
		void setLongitude(const Coordinate& longitude);

		bool operator==(const Site& rhs) const;
		bool operator!=(const Site& rhs) const;

		Site& operator=(const Site& rhs);

	private:
		Coordinate latitude;
		Coordinate longitude;
	};
}