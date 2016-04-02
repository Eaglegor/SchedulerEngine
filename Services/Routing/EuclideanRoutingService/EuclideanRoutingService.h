#pragma once

#include <Engine/Engine/Services/RoutingService.h>

#include <EuclideanRoutingService_export.h>

namespace Scheduler {
	
	class EUCLIDEANROUTINGSERVICE_EXPORT EuclideanRoutingService : public RoutingService {
	public:
		virtual Route calculateRoute(const Location &from, const Location &to,
									 const RoutingProfile &routing_profile) override;
	};
}