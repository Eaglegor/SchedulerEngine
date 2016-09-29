#pragma once

#include <Engine/Engine/Services/RoutingService.h>

#include <TaillardRoutingService_export.h>

namespace Scheduler {
	
	class TAILLARDROUTINGSERVICE_EXPORT TaillardRoutingService : public RoutingService {
	public:
		virtual Route calculateRoute(const Site &from, const Site &to,
									 const RoutingProfile &routing_profile) override;
	};
}