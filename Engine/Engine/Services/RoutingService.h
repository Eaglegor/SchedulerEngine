#pragma once

#include <Engine/Concepts/Route.h>

namespace Scheduler
{
	class Site;
	class RoutingProfile;
	
	class RoutingService {
	public:
		virtual ~RoutingService() {}
		virtual Route calculateRoute(const Site &from, const Site &to, const RoutingProfile &routing_profile) const = 0;
	};
}