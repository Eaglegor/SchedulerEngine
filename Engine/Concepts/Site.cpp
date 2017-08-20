#include "Site.h"

namespace Scheduler
{

	Site::Site( )
	    : latitude(0),
	      longitude(0)
	{
	}

	Site::Site(const Site& rhs)
	    : latitude(rhs.latitude),
	      longitude(rhs.longitude)
	{
	}

	Site::Site(Coordinate latitude, Coordinate longitude)
	    : latitude(latitude),
	      longitude(longitude)
	{
	}

	Coordinate Site::getLatitude( ) const
	{
		return latitude;
	}

	Coordinate Site::getLongitude( ) const
	{
		return longitude;
	}

	void Site::setLatitude(const Coordinate& latitude)
	{
		this->latitude = latitude;
	}

	void Site::setLongitude(const Coordinate& longitude)
	{
		this->longitude = longitude;
	}

	bool Site::operator==(const Site& rhs) const
	{
		return this->latitude == rhs.latitude && this->longitude == rhs.longitude;
	}

	bool Site::operator!=(const Site& rhs) const
	{
		return !(*this == rhs);
	}

	Site& Site::operator=(const Site& rhs)
	{
		this->latitude  = rhs.latitude;
		this->longitude = rhs.longitude;
		return *this;
	}
}