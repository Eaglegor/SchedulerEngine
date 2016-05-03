#include "DefaultRouteActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/Engine/Services/RoutingService.h>

namespace Scheduler
{
	DefaultRouteActualizationAlgorithm::DefaultRouteActualizationAlgorithm(RoutingService* routing_service, const RoutingProfile& default_routing_profile):
		routing_service(routing_service),
		default_routing_profile(default_routing_profile)
	{
	}

	DefaultRouteActualizationAlgorithm::DefaultRouteActualizationAlgorithm(RoutingService* routing_service):
	routing_service(routing_service)
	{
	}

	void DefaultRouteActualizationAlgorithm::actualize(Stop* stop)
	{
		if(!stop->getNextStop())
		{
			stop->setNextRoute(Route(stop->getLocation(), stop->getLocation(), Distance(0), Duration(0)));
		}
		else
		{
			if(stop->getRun()->getVehicle())
			{
				stop->setNextRoute(routing_service->calculateRoute(stop->getLocation(), stop->getNextStop()->getLocation(), stop->getRun()->getVehicle()->getRoutingProfile()));
			}
			else
			{
				stop->setNextRoute(routing_service->calculateRoute(stop->getLocation(), stop->getNextStop()->getLocation(), default_routing_profile));
			}
		}
	}
}
