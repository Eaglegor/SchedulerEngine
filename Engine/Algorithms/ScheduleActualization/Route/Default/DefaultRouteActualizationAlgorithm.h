#pragma once

#include <Engine/SceneManager/Algorithms/Actualization/RouteActualizationAlgorithm.h>


#include <Engine/Concepts/RoutingProfile.h>

namespace Scheduler
{
	class RoutingService;

	class DefaultRouteActualizationAlgorithm : public RouteActualizationAlgorithm
	{
	public:
		explicit DefaultRouteActualizationAlgorithm(const RoutingService& routing_service);
		DefaultRouteActualizationAlgorithm(const RoutingService& routing_service, const RoutingProfile& default_routing_profile);

		static constexpr const char* staticGetName( )
		{
			return "Default";
		}
		virtual const char* getName( ) const override
		{
			return staticGetName( );
		};

		virtual void actualize(Stop& stop) const override;

	private:
		const RoutingService& routing_service;
		RoutingProfile default_routing_profile;
	};
}
