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
#include "SuggestedEdge.h"
#include "EdgeSuggestor.h"
#include "EdgeIntroducer.h"
#include "GCBIEdgeSuggestor.h"
#include "GCBIEdgeIntroducer.h"

namespace Scheduler
{
	GCBITSPSolver::GCBITSPSolver():
		routing_service(nullptr),
		cost_function(nullptr),
		logger(LoggingManager::getLogger("GCBITSPSolver"))
	{
	}

	void GCBITSPSolver::optimize(Schedule* schedule) const
	{
		if (routing_service == nullptr || cost_function == nullptr) {
			if (routing_service == nullptr) SIMPLE_LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if (cost_function == nullptr) SIMPLE_LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		SIMPLE_LOG_INFO(logger, "Multirun TSP started");

		for(Run* r: schedule->getRuns())
		{
			optimize(r);
		}

		SIMPLE_LOG_INFO(logger, "Multirun TSP finished");
	}

	void GCBITSPSolver::optimize(Run* run) const
	{
		if (routing_service == nullptr || cost_function == nullptr)
		{
			if (routing_service == nullptr) SIMPLE_LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if (cost_function == nullptr) SIMPLE_LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		SIMPLE_LOG_INFO(logger, "Single run TSP started");

		SIMPLE_LOG_TRACE(logger, "Initializing");

		bool finished = false;

		Cost current_cost = cost_function->calculateCost(run->getSchedule());

		SceneEditor scene_editor;
		
		GCBIEdgeSuggestor suggestor(run, routing_service);
		GCBIEdgeIntroducer introducer(run, cost_function, scene_editor);

		SIMPLE_LOG_TRACE(logger, "Starting main cycle");

		while (!finished)
		{
			SIMPLE_LOG_TRACE(logger, "Starting next iteration");
			finished = true;
			while (suggestor.hasNext())
			{
				std::vector<SuggestedEdge> edges = suggestor.next();
				LOG_DEBUG(logger, "{} new edges suggested", edges.size());
				for (SuggestedEdge edge : edges)
				{
					LOG_DEBUG(logger, "Suggested edge: {}-{}", edge.from_index, edge.to_index);
					if (edge.from_index == edge.to_index) continue;
					if (edge.from_index == 0 && edge.to_index == run->getWorkStops().size() + 1) continue;
					if (edge.from_index == run->getWorkStops().size() + 1) continue;
					if (edge.to_index == 0) continue;
					SIMPLE_LOG_TRACE(logger, "Trying to introduce suggested edge");
					if (introducer.introduce(edge))
					{
						SIMPLE_LOG_TRACE(logger, "Introduction successful. Commiting changes");
						scene_editor.commit();
						finished = false;
						break;
					}
					else
					{
						SIMPLE_LOG_TRACE(logger, "Introduction failed. Rolling changes back");
						scene_editor.rollbackAll();
					}
				}
			}
			SIMPLE_LOG_TRACE(logger, "Resetting suggestor");
			suggestor.reset();
		}

		SIMPLE_LOG_INFO(logger, "Single run TSP finished");
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
