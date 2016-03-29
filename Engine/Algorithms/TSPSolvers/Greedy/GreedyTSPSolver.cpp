#include "GreedyTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>

namespace Scheduler
{
	GreedyTSPSolver::GreedyTSPSolver():
        routing_service(nullptr)
	{
	}

    void GreedyTSPSolver::optimize(Schedule* schedule) const
	{
        if (!routing_service) return; // We don't have a metric to optimize - so we can't

		for(Run* run : schedule->getRuns())
		{
			optimize(run);
		}
	}

	void GreedyTSPSolver::optimize(Run* run) const
	{
        if (!routing_service) return; // We don't have a metric to optimize - so we can't

        auto &stops = run->getWorkStops();
        const RoutingProfile &routing_profile = run->getVehicle()->getRoutingProfile();
        auto location = run->getStartStop()->getLocation();
        SceneEditor scene_editor;
        for (size_t i = 0; i < stops.size() - 1; ++i) {
            size_t nearest_element_idx = i;
            auto min_distance = routing_service->calculateRoute(location, stops[nearest_element_idx]->getLocation(), routing_profile).getDistance();
            for (size_t j = i + 1; j < stops.size(); ++j) {
                const auto distance = routing_service->calculateRoute(location, stops[j]->getLocation(), routing_profile).getDistance();
                if (distance < min_distance) {
                    min_distance = distance;
                    nearest_element_idx = j;
                }
            }

            location = stops[nearest_element_idx]->getLocation();
            scene_editor.performAction<SwapRunWorkStops>(run, i, nearest_element_idx);
        }
	}

    void GreedyTSPSolver::setRoutingService(RoutingService* routing_service)
	{
        this->routing_service = routing_service;
	}
}
