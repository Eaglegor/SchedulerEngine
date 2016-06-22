#include "GCBITSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/RotateWorkStopsSubsequence.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Stop.h>
#include <algorithm>
#include <queue>

namespace Scheduler
{
	GCBITSPSolver::GCBITSPSolver():
		routing_service(nullptr),
		cost_function(nullptr)
	{
	}

	void GCBITSPSolver::optimize(Schedule* schedule) const
	{
		if (routing_service == nullptr || cost_function == nullptr) return;
		for(Run* r: schedule->getRuns())
		{
			optimize(r);
		}
	}

	struct LinkEstimate
	{
		Location from;
		Location to;
		float cost = 0;

		bool operator<(const LinkEstimate& rhs) const
		{
			return cost < rhs.cost;
		}
	};

	std::vector<LinkEstimate> generateLinksEstimate(Run* run, RoutingService* routing_service)
	{
		std::vector<Location> locations(run->getWorkStops().size() + 2);
		for (Stop* stop = run->getStartStop(); stop != run->getEndStop()->getNextStop(); stop = stop->getNextStop())
		{
			locations.push_back(stop->getLocation());
		}
		
		std::vector<LinkEstimate> link_estimates;

		for (size_t i = 0; i < locations.size(); ++i)
		{
			const Location& location1 = locations[i];
			if (std::find(locations.begin(), locations.begin() + i, location1) != locations.begin() + i) continue;
			LinkEstimate self_estimate;
			self_estimate.from = location1;
			self_estimate.to = location1;
			self_estimate.cost = 0.0f;
			link_estimates.push_back(self_estimate);
			for (size_t j = 0; j < locations.size(); ++j)
			{
				const Location& location2 = locations[j];
				if (location1 == location2) continue;
				LinkEstimate estimate;
				estimate.from = location1;
				estimate.to = location2;
				estimate.cost = routing_service->calculateRoute(location1, location2, run->getVehicle()->getRoutingProfile()).getDistance().getValue();
				link_estimates.push_back(estimate);
			}
		}


		return link_estimates;
	}

	void GCBITSPSolver::optimize(Run* run) const
	{
		if (routing_service == nullptr || cost_function == nullptr) return;

		std::vector<LinkEstimate> estimates = generateLinksEstimate(run, routing_service);
		std::sort(estimates.begin(), estimates.end());

		Cost currentCost = cost_function->calculateCost(run->getSchedule());

		SceneEditor scene_editor;
		
		ImmutableVector<Run*>::const_iterator run_iter = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);

		bool finished = false;

		while (!finished)
		{
			finished = true;
			bool iteration_finished = false;
			Distance max_distance = run->getStartStop()->getNextRoute().getDistance();
			for (WorkStop* stop : run->getWorkStops())
			{
				if (max_distance < stop->getNextRoute().getDistance())
				{
					max_distance = stop->getNextRoute().getDistance();
				}
			}

			for (const LinkEstimate& estimate : estimates)
			{
				if (estimate.cost > max_distance.getValue()) iteration_finished = true;
				if (iteration_finished) break;
				if (run->getStartStop()->getLocation() == estimate.from)
				{
					ImmutableVector<WorkStop*>::const_iterator to_stop = std::find_if(run->getWorkStops().begin(), run->getWorkStops().end(), [&](const Stop* stop) {return stop->getLocation() == estimate.to;});
					if (to_stop == run->getWorkStops().end()) continue;
					if (run->getStartStop()->getNextStop() == *to_stop) continue;
					for (auto iter = to_stop + 1; iter != run->getWorkStops().end(); ++iter)
					{
						if (iteration_finished) break;
						scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, run->getWorkStops().begin(), to_stop, iter);
						Cost newCost = cost_function->calculateCost(run->getSchedule());
						if (newCost < currentCost)
						{
							finished = false;
							iteration_finished = true;
							currentCost = newCost;
							scene_editor.commit();
						}
						else
						{
							scene_editor.rollbackAll();
						}
					}
				}
				else
				{
					ImmutableVector<WorkStop*>::const_iterator from_stop = std::find_if(run->getWorkStops().begin(), run->getWorkStops().end(), [&](const Stop* stop) {return stop->getLocation() == estimate.from; });
					if (from_stop == run->getWorkStops().end()) continue;
					ImmutableVector<WorkStop*>::const_iterator to_stop = std::find_if(run->getWorkStops().begin(), run->getWorkStops().end(), [&](const Stop* stop) {return stop->getLocation() == estimate.to && stop != *from_stop; });
					if (to_stop != run->getWorkStops().end())
					{
						if ((*from_stop)->getNextStop() == *to_stop) continue;
						if (std::distance(from_stop, to_stop) > 0)
						{
							for (auto iter = to_stop + 1; iter != run->getWorkStops().end(); ++iter)
							{
								if (iteration_finished) break;
								scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, from_stop + 1, to_stop, iter);
								Cost newCost = cost_function->calculateCost(run->getSchedule());
								if (newCost < currentCost)
								{
									finished = false;
									iteration_finished = true;
									currentCost = newCost;
									scene_editor.commit();
								}
								else
								{
									scene_editor.rollbackAll();
								}
							}

							for (auto iter = run->getWorkStops().begin(); iter != from_stop + 1; ++iter)
							{
								if (iteration_finished) break;
								scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, from_stop + 1, to_stop, iter);
								Cost newCost = cost_function->calculateCost(run->getSchedule());
								if (newCost < currentCost)
								{
									finished = false;
									iteration_finished = true;
									currentCost = newCost;
									scene_editor.commit();
								}
								else
								{
									scene_editor.rollbackAll();
								}
							}
						}
						else
						{
							for (auto iter = to_stop + 1; iter != from_stop + 1; ++iter)
							{
								if (iteration_finished) break;
								scene_editor.performAction<RotateWorkStopsSubsequence>(run_iter, to_stop, iter, from_stop + 1);
								Cost newCost = cost_function->calculateCost(run->getSchedule());
								if (newCost < currentCost)
								{
									finished = false;
									iteration_finished = true;
									currentCost = newCost;
									scene_editor.commit();
								}
								else
								{
									scene_editor.rollbackAll();
								}
							}
						}
					}
					else
					{
						if(estimate.to == run->getEndStop()->getLocation())
						{
							if ((*from_stop)->getNextStop() == run->getEndStop()) continue;
							for (auto iter = run->getWorkStops().begin(); iter != from_stop + 1; ++iter)
							{
								if (iteration_finished) break;
								scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, from_stop + 1, run->getWorkStops().end(), iter);
								Cost newCost = cost_function->calculateCost(run->getSchedule());
								if (newCost < currentCost)
								{
									finished = false;
									iteration_finished = true;
									currentCost = newCost;
									scene_editor.commit();
								}
								else
								{
									scene_editor.rollbackAll();
								}
							}
						}
					}
				}
			}
		}

		return;
	}

	void GCBITSPSolver::setRoutingService(RoutingService* routing_service)
	{
		this->routing_service = routing_service;
	}

	void GCBITSPSolver::setCostFunction(ScheduleCostFunction* cost_function)
	{
		this->cost_function = cost_function;
	}
}
