#pragma once

#include <Engine/Core/Services/RoutingService.h>

#include <CrowFlyRoutingService_export.h>

namespace Scheduler {
	
	class CROWFLYROUTINGSERVICE_EXPORT CrowFlyRoutingService : public RoutingService {
	public:
		virtual Route calculateRoute(const Location &from, const Location &to,
									 const RoutingProfile &routing_profile) override;
	};
}