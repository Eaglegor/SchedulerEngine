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

	void GCBITSPSolver::optimize(Run* run) const
	{
		if (routing_service == nullptr || cost_function == nullptr) return;

		bool finished = false;

		Cost current_cost = cost_function->calculateCost(run->getSchedule());

		SceneEditor scene_editor;

		GCBIEdgeSuggestor suggestor(run, routing_service);
		GCBIEdgeIntroducer introducer(run, cost_function, scene_editor);

		while (!finished)
		{
			finished = true;
			while (suggestor.hasNext())
			{
				std::vector<SuggestedEdge> edges = suggestor.next();
				for (SuggestedEdge edge : edges)
				{
					if (edge.from_index == edge.to_index) continue;
					if (edge.from_index == 0 && edge.to_index == run->getWorkStops().size() + 1) continue;
					if (edge.from_index == run->getWorkStops().size() + 1) continue;
					if (introducer.introduce(edge))
					{
						scene_editor.commit();
						finished = false;
						break;
					}
					else
					{
						scene_editor.rollbackAll();
					}
				}
			}
			suggestor.reset();
		}
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
