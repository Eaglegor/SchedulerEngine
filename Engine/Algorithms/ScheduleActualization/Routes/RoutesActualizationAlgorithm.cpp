#include "RoutesActualizationAlgorithm.h"
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/Concepts/Duration.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Views/RunStopsView.h>
#include <Engine/Engine/Services/RoutingService.h>

namespace Scheduler
{

	RoutesActualizationAlgorithm::RoutesActualizationAlgorithm(Schedule *schedule, RoutingService* routing_service):
	ScheduleActualizationAlgorithm(schedule),
	dirty_flag(true),
	routing_service(routing_service)
	{
	}

	void RoutesActualizationAlgorithm::onOperationAdded(const Stop *stop, const Operation *operation)
	{
		// Doesn't affect route
	}

	void RoutesActualizationAlgorithm::onOperationRemoved(const Stop *stop)
	{
		// Doesn't affect route
	}

	void RoutesActualizationAlgorithm::onStopNextRouteChanged(const Stop *stop)
	{
		// Don't need to check
	}

	void RoutesActualizationAlgorithm::onStopAdded(const Run *run, const Stop *stop, size_t index)
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::onStopReplaced(const Run * run, const Stop * new_stop, size_t index)
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::onStopRemoved(const Run *run)
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::onRunVehicleChanged(const Run *run, const Vehicle *vehicle)
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::onRunAdded(const Run *run, size_t index)
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::onRunRemoved()
	{
		dirty_flag = true;
	}

	void RoutesActualizationAlgorithm::actualize()
	{
		if(!dirty_flag) return;

		for(Run *r : schedule->getRuns())
		{
			RunStopsView stops(r);

			for (int i = 0; i < stops.size() - 1; ++i)
			{
				if(!stops[i]->hasActualRoute())
				{
					if(r->getVehicle() == nullptr)
					{
						stops[i]->setNextRoute(Route());
					}
					else
					{
						stops[i]->setNextRoute(routing_service->calculateRoute(stops[i]->getLocation(), stops[i+1]->getLocation(), r->getVehicle()->getRoutingProfile()));
					}
				}
			}
		}
		
		dirty_flag = false;
	}
}
