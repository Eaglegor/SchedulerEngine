#pragma once

#include "Distance.h"
#include "Duration.h"
#include "Location.h"

#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class SCHEDULERENGINE_EXPORT Route
	{
	public:
		Route();
		Route(const Route &rhs);
		Route(const Location &from, const Location &to, const Distance &distance, const Duration &duration);

		const Location& getFrom() const;
		const Location& getTo() const;

		void setFrom(const Location &from);
		void setTo(const Location &to);

		const Distance& getDistance() const;
		void setDistance(const Distance &distance);

		const Duration& getDuration() const;
		void setDuration(const Duration &duration);

		bool operator==(const Route &rhs) const;
		bool operator!=(const Route &rhs) const;

		Route& operator=(const Route &rhs);

	private:
		Location from;
		Location to;
	
		Distance distance;
		Duration duration;
	};
}