#pragma once

#include <Engine/PluginAPI/Services/RoutingService.h>

namespace Scheduler {
	class CrowFlyRoutingService : public RoutingService {
	public:
		virtual Route calculateRoute(const Location &from, const Location &to,
									 const RoutingProfile &routing_profile) override;
	};
}