#pragma once

#include <Engine/Routing/RoutingService.h>

#include <CrowFlyRoutingService_export.h>

namespace Scheduler
{

	class CROWFLYROUTINGSERVICE_EXPORT CrowFlyRoutingService : public RoutingService
	{
	public:
		virtual Route calculateRoute(const Site& from, const Site& to,
		                             const RoutingProfile& routing_profile) const override;
	};
}