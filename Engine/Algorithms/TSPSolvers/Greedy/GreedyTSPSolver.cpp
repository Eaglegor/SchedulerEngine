#include "GreedyTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <algorithm>

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
		if (stops.empty()) return;

		auto run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
        const RoutingProfile &routing_profile = run->getVehicle()->getRoutingProfile();
        auto location = run->getStartStop()->getLocation();
        SceneEditor scene_editor;
        for (auto stop_i = stops.begin(); stop_i != std::prev(stops.end()); ++stop_i) {
            auto nearest_element_iter = stop_i;
            auto min_distance = routing_service->calculateRoute(location, (*nearest_element_iter)->getLocation(), routing_profile).getDistance();
            for (auto stop_j = std::next(stop_i); stop_j != stops.end(); ++stop_j) {
                const auto distance = routing_service->calculateRoute(location, (*stop_j)->getLocation(), routing_profile).getDistance();
                if (distance < min_distance) {
                    min_distance = distance;
					nearest_element_iter = stop_j;
                }
            }

            location = (*nearest_element_iter)->getLocation();
            scene_editor.performAction<SwapRunWorkStops>(run_iter, stop_i, nearest_element_iter);
			scene_editor.commit();
        }
	}

    void GreedyTSPSolver::setRoutingService(RoutingService* routing_service)
	{
        this->routing_service = routing_service;
	}
}
