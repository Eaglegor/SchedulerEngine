#include "GreedyTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Utils/InvariantWorkStopsList.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <algorithm>

namespace Scheduler
{
	GreedyTSPSolver::GreedyTSPSolver():
        logger(LoggingManager::getLogger("GreedyTSPSolver"))
	{
	}

    void GreedyTSPSolver::optimize(Schedule& schedule) const
	{
        if (!routing_service) return;

		for(Run& run : schedule.getRuns())
		{
			optimize(run);
		}
	}

	void printStopsOrder(const std::string &prefix, Run& run, Logger& logger)
	{
		std::string order;
		for(WorkStop& stop : run.getWorkStops())
		{
			order += std::to_string(stop.getOperation().getId());
			order += " ";
		}
		LOG_DEBUG(logger, "{}: {}", prefix, order);	
	}
	
	void GreedyTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "GreedyTSPSolver::optimize(Run&)", logger);
		
        if (!routing_service) return;
		
		if(DEBUG_LOGGING_ENABLED) printStopsOrder("Initial order", run, logger);
		
		if(run.getWorkStops().empty()) return;
		
		InvariantWorkStopsList stops(run.getWorkStops());

        const RoutingProfile &routing_profile = run.getVehicle()->getRoutingProfile();
		
        auto location = run.getStartStop().getLocation().getSite();
		
        SceneEditor scene_editor;
		
        for (auto stop_i = stops.begin(); stop_i != std::prev(stops.end()); ++stop_i) {
            auto nearest_element_iter = stop_i;
            auto min_distance = routing_service->calculateRoute(location, nearest_element_iter->getLocation().getSite(), routing_profile).getDistance();
            for (auto stop_j = std::next(stop_i); stop_j != stops.end(); ++stop_j) {
                const auto distance = routing_service->calculateRoute(location, stop_j->getLocation().getSite(), routing_profile).getDistance();
                if (distance < min_distance) {
                    min_distance = distance;
					nearest_element_iter = stop_j;
                }
            }

            location = nearest_element_iter->getLocation().getSite();
            scene_editor.performAction<SwapWorkStops>(run, run.findWorkStop(*stop_i), run.findWorkStop(*nearest_element_iter));
			scene_editor.commit();
			stops = run.getWorkStops();
			
			if(DEBUG_LOGGING_ENABLED) printStopsOrder("Order changed", run, logger);
        }
	}

    void GreedyTSPSolver::setRoutingService(const RoutingService& routing_service)
	{
        this->routing_service = routing_service;
	}
}
