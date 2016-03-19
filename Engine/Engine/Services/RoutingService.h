#pragma once

#include <Engine/Concepts/Route.h>

namespace Scheduler
{
	class Location;
	class RoutingProfile;
	
	class RoutingService {
	public:
		virtual ~RoutingService() {}
		virtual Route calculateRoute(const Location &from, const Location &to, const RoutingProfile &routing_profile) = 0;
	};
}