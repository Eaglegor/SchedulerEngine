#pragma once

#include <Engine/Concepts/Basic/Route.h>
#include <SchedulerEngine_export.h>

namespace Scheduler
{
	class Location;
	class RoutingProfile;

	class SCHEDULERENGINE_EXPORT RoutingService
	{
	public:
		virtual ~RoutingService(){};

		virtual Route calculateRoute(const Location& from, const Location& to, const RoutingProfile& routing_profile) = 0;
	};
}