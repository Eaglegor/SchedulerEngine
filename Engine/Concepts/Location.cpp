#include "Location.h"

namespace Scheduler
{

	Location::Location() : latitude(0),
						   longitude(0)
	{ }

	Location::Location(const Location &rhs) : latitude(rhs.latitude),
											  longitude(rhs.longitude)
	{ }

	Location::Location(Coordinate latitude, Coordinate longitude) : latitude(latitude),
																	longitude(longitude)
	{ }

	Coordinate Location::getLatitude() const
	{
		return latitude;
	}

	Coordinate Location::getLongitude() const
	{
		return longitude;
	}

	void Location::setLatitude(const Coordinate &latitude)
	{
		this->latitude = latitude;
	}

	void Location::setLongitude(const Coordinate &longitude)
	{
		this->longitude = longitude;
	}

	bool Location::operator==(const Location &rhs) const
	{
		return this->latitude == rhs.latitude && this->longitude == rhs.longitude;
	}

	bool Location::operator!=(const Location &rhs) const
	{
		return !(*this == rhs);
	}

	Location& Location::operator=(const Location &rhs)
	{
		this->latitude = rhs.latitude;
		this->longitude = rhs.longitude;
		return *this;
	}
}