#include "GreedyTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <iostream>

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
        for (size_t idx = 0; idx < stops.size() - 1; ++idx) {
            const auto nearestElement = std::min_element(stops.begin() + idx,
                                                         stops.end(),
                                                         [&] (WorkStop* lhs, WorkStop* rhs) {
                    const auto lhs_distance = routing_service->calculateRoute(location, lhs->getLocation(), routing_profile).getDistance();
                    const auto rhs_distance = routing_service->calculateRoute(location, rhs->getLocation(), routing_profile).getDistance();
                    return lhs_distance < rhs_distance;
            });
            location = (*nearestElement)->getLocation();
            scene_editor.performAction<SwapRunWorkStops>(run, idx, nearestElement - stops.begin());
        }
	}

    void GreedyTSPSolver::setRoutingService(RoutingService* routing_service)
	{
        this->routing_service = routing_service;
	}
}
