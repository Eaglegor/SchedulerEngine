#include "Route.h"

namespace Scheduler
{

	Route::Route()
	{ }

	Route::Route(const Route &rhs) : from(rhs.from),
									 to(rhs.to),
									 distance(rhs.distance),
									 duration(rhs.duration)
	{ }

	Route::Route(const Location &from, const Location &to, const Distance &distance, const Duration &duration)
			: from(from),
			  to(to),
			  distance(distance),
			  duration(duration)
	{ }

	const Location& Route::getFrom() const
	{
		return from;
	}

	const Location& Route::getTo() const
	{
		return to;
	}

	const Distance& Route::getDistance() const
	{
		return distance;
	}

	const Duration& Route::getDuration() const
	{
		return duration;
	}

	void Route::setFrom(const Location &from)
	{
		this->from = from;
	}

	void Route::setTo(const Location &to)
	{
		this->to = to;
	}

	void Route::setDistance(const Distance &distance)
	{
		this->distance = distance;
	}

	void Route::setDuration(const Duration &duration)
	{
		this->duration = duration;
	}

	Route& Route::operator=(const Route &rhs)
	{
		this->from = rhs.from;
		this->to = rhs.to;
		this->distance = rhs.distance;
		this->duration = rhs.duration;

		return *this;
	}

	bool Route::operator==(const Route &rhs) const
	{
		return from == rhs.from && to == rhs.to && distance == rhs.distance && duration == rhs.duration;
	}

	bool Route::operator!=(const Route &rhs) const
	{
		return !(*this == rhs);
	}
}