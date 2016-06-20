#include "GreedyCircularBlockInsertionTSPSolver.h"
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/SwapRunWorkStops.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/Concepts/Route.h>
#include <Engine/SceneManager/WorkStop.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/RotateWorkStopsSubsequence.h>
#include <Engine/SceneManager/Stop.h>
#include <algorithm>

namespace Scheduler
{
	GreedyCircularBlockInsertionTSPSolver::GreedyCircularBlockInsertionTSPSolver():
		routing_service(nullptr),
		cost_function(nullptr)
	{
	}

	void GreedyCircularBlockInsertionTSPSolver::optimize(Schedule* schedule) const
	{
		if (routing_service == nullptr || cost_function == nullptr) return;
		for(Run* r: schedule->getRuns())
		{
			optimize(r);
		}
	}

	struct LinkEstimate
	{
		size_t from_index = 0;
		size_t to_index = 0;
		float estimate = 0;
	};

	std::vector<LinkEstimate> generateLinksEstimate(Run* run, RoutingService* routing_service)
	{
		std::vector<LinkEstimate> estimates;
		RoutingProfile rp;
		for (auto iter1 = run->getWorkStops().begin(); iter1 != run->getWorkStops().end(); ++iter1)
		{
			LinkEstimate start_work;
			start_work.from_index = 0;
			start_work.to_index = std::distance(run->getWorkStops().begin(), iter1) + 1;
			start_work.estimate = routing_service->calculateRoute(run->getStartStop()->getLocation(), (*iter1)->getLocation(), rp).getDistance().getValue();
			estimates.push_back(start_work);
			
			for (auto iter2 = run->getWorkStops().begin(); iter2 != run->getWorkStops().end(); ++iter2)
			{
				if (iter1 == iter2) continue;

				LinkEstimate work_work;
				work_work.from_index = std::distance(run->getWorkStops().begin(), iter1) + 1;
				work_work.to_index = std::distance(run->getWorkStops().begin(), iter2) + 1;
				work_work.estimate = routing_service->calculateRoute((*iter1)->getLocation(), (*iter2)->getLocation(), rp).getDistance().getValue();
				estimates.push_back(work_work);
			}

			LinkEstimate work_end;
			work_end.from_index = std::distance(run->getWorkStops().begin(), iter1) + 1;
			work_end.to_index = run->getWorkStops().size() + 1;
			work_end.estimate = routing_service->calculateRoute((*iter1)->getLocation(), run->getEndStop()->getLocation(), rp).getDistance().getValue();
			estimates.push_back(work_end);
		}

		std::sort(estimates.begin(), estimates.end(), [](const LinkEstimate& l1, const LinkEstimate& l2) {return l1.estimate < l2.estimate; });
		return estimates;
	}

	void GreedyCircularBlockInsertionTSPSolver::optimize(Run* run) const
	{
		if (routing_service == nullptr || cost_function == nullptr) return;

		auto run_iterator = std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run);
		Cost best_cost(std::numeric_limits<float>::max());
		SceneEditor scene_editor;
		bool finished = false;

		while(!finished)
		{
			finished = true;
			bool iteration_finished = false;
			std::vector<LinkEstimate> estimates = generateLinksEstimate(run, routing_service);
			
			for(const LinkEstimate& estimate : estimates)
			{
				if (iteration_finished) break;
				Stop* fromStop = nullptr;
				Stop* toStop = nullptr;
				if (estimate.from_index == 0) fromStop = run->getStartStop();
				else fromStop = run->getWorkStops()[estimate.from_index - 1];

				if (estimate.to_index >= run->getWorkStops().size()) toStop = run->getEndStop();
				else toStop = run->getWorkStops()[estimate.to_index - 1];

				if (fromStop->getNextStop() != toStop)
				{
					if (estimate.from_index < estimate.to_index)
					{
						
					}
				}
			}
		}
	}

	void GreedyCircularBlockInsertionTSPSolver::setRoutingService(RoutingService* routing_service)
	{
		this->routing_service = routing_service;
	}

	void GreedyCircularBlockInsertionTSPSolver::setCostFunction(ScheduleCostFunction* cost_function)
	{
		this->cost_function = cost_function;
	}
}
