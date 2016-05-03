#pragma once

#include <Engine/SceneManager/Extensions/RouteActualizationAlgorithm.h>

#include <DefaultRouteActualizationAlgorithm_export.h>
#include <Engine/Concepts/RoutingProfile.h>

namespace Scheduler
{
	class RoutingService;

	/// Updates stops durations based on the operations scheduled to be performed at the stop
	class DEFAULTROUTEACTUALIZATIONALGORITHM_EXPORT DefaultRouteActualizationAlgorithm : public RouteActualizationAlgorithm
	{
	public:
		DefaultRouteActualizationAlgorithm(RoutingService* routing_service);
		DefaultRouteActualizationAlgorithm(RoutingService* routing_service, const RoutingProfile &default_routing_profile);

		static constexpr const char* staticGetName() { return "Default"; }
		virtual const char* getName() const { return staticGetName(); };

		virtual void actualize(Stop* stop) override;
		
	private:
		RoutingService* routing_service;
		RoutingProfile default_routing_profile;
	};
}
