#include "DefaultRouteActualizationAlgorithm.h"
#include <Engine/Concepts/Duration.h>
#include <Engine/Routing/RoutingService.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>

namespace Scheduler
{
	DefaultRouteActualizationAlgorithm::DefaultRouteActualizationAlgorithm(const RoutingService& routing_service, const RoutingProfile& default_routing_profile)
	    : routing_service(routing_service),
	      default_routing_profile(default_routing_profile)
	{
	}

	DefaultRouteActualizationAlgorithm::DefaultRouteActualizationAlgorithm(const RoutingService& routing_service)
	    : routing_service(routing_service)
	{
	}

	void DefaultRouteActualizationAlgorithm::actualize(Stop& stop) const
	{
		Run& r           = stop.getRun( );
		Schedule& s      = r.getSchedule( );
		auto iter        = r.findStop(stop);
		auto& stops_list = s.getStops( );
		if(std::next(iter) == stops_list.end( ))
		{
			stop.setNextRoute(Route(stop.getLocation( ).getSite( ), stop.getLocation( ).getSite( ), Distance(0), Duration(0)));
		}
		else
		{
			if(stop.getRun( ).getVehicle( ))
			{
				stop.setNextRoute(routing_service.calculateRoute(stop.getLocation( ).getSite( ), std::next(iter)->getLocation( ).getSite( ), stop.getRun( ).getVehicle( )->getRoutingProfile( )));
			}
			else
			{
				stop.setNextRoute(routing_service.calculateRoute(stop.getLocation( ).getSite( ), std::next(iter)->getLocation( ).getSite( ), default_routing_profile));
			}
		}
	}
}
