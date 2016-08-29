#include "DefaultRouteActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Location.h>
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
		if(!stop->next())
		{
			stop->setNextRoute(Route(stop->getLocation().getSite(), stop->getLocation().getSite(), Distance(0), Duration(0)));
		}
		else
		{
			if(stop->getRun()->getVehicle())
			{
				stop->setNextRoute(routing_service->calculateRoute(stop->getLocation().getSite(), stop->next()->getLocation().getSite(), stop->getRun()->getVehicle()->getRoutingProfile()));
			}
			else
			{
				stop->setNextRoute(routing_service->calculateRoute(stop->getLocation().getSite(), stop->next()->getLocation().getSite(), default_routing_profile));
			}
		}
	}
}
