#include "SuIntTSPSolver.h"
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
#include "DistanceRatingEdgeSuggestor.h"
#include "EdgeIntroducers/CompositeEdgeIntroducer.h"
#include "BetterEdgeSuggestor.h"
#include "EdgeIntroducers/DirectEdgeIntroducer.h"
#include "EdgeIntroducers/CircularEdgeIntroducer.h"
#include "EdgeIntroducers/ReverseEdgeIntroducer.h"

namespace Scheduler
{
	SuIntTSPSolver::SuIntTSPSolver():
		routing_service(nullptr),
		cost_function(nullptr),
		logger(LoggingManager::getLogger("SuIntTSPSolver"))
	{
	}

	void SuIntTSPSolver::optimize(Schedule* schedule) const
	{
		if (routing_service == nullptr || cost_function == nullptr) {
			if (routing_service == nullptr) SIMPLE_LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if (cost_function == nullptr) SIMPLE_LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		SIMPLE_LOG_DEBUG(logger, "Multirun TSP started");

		for(Run* r: schedule->getRuns())
		{
			optimize(r);
		}

		SIMPLE_LOG_DEBUG(logger, "Multirun TSP finished");
	}

	void SuIntTSPSolver::optimize(Run* run) const
	{
		if (routing_service == nullptr || cost_function == nullptr)
		{
			if (routing_service == nullptr) SIMPLE_LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if (cost_function == nullptr) SIMPLE_LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		SIMPLE_LOG_DEBUG(logger, "Single run TSP started");

		SIMPLE_LOG_TRACE(logger, "Initializing");

		bool finished = false;

		SceneEditor scene_editor;
		
		std::unique_ptr<EdgeSuggestor> suggestor;

		switch(edge_suggestor_type)
		{
			case EdgeSuggestorType::BETTER_EDGE:
			{
				suggestor.reset(new BetterEdgeSuggestor(run, routing_service));
				break;
			}
			case EdgeSuggestorType::DISTANCE_RATING:
			{
				suggestor.reset(new DistanceRatingEdgeSuggestor(run, routing_service));
				break;
			}
		}
		
		CompositeEdgeIntroducer edge_introducer(run, cost_function, scene_editor);
		for(const EdgeIntroducerType& introducer_type : edge_introducers_types)
		{
			switch(introducer_type)
			{
				case EdgeIntroducerType::CIRCULAR:
				{
					edge_introducer.addIntroducer<CircularEdgeIntroducer>(run, cost_function, scene_editor);
					break;
				}
				case EdgeIntroducerType::DIRECT:
				{
					edge_introducer.addIntroducer<DirectEdgeIntroducer>(run, cost_function, scene_editor);
					break;
				}
				case EdgeIntroducerType::REVERSE:
				{
					edge_introducer.addIntroducer<ReverseEdgeIntroducer>(run, cost_function, scene_editor);
					break;
				}
			}
		}
		
		assert(suggestor);

		SIMPLE_LOG_TRACE(logger, "Starting main cycle");

		while (!finished)
		{
			SIMPLE_LOG_TRACE(logger, "Starting next iteration");
			finished = true;
			while (suggestor->hasNext())
			{
				std::vector<SuggestedEdge> edges = suggestor->next();
				for (SuggestedEdge edge : edges)
				{
					LOG_DEBUG(logger, "Suggested edge: {}-{}", edge.from_index, edge.to_index);
					if (edge.from_index == edge.to_index) continue;
					if (edge.from_index == 0 && edge.to_index == run->getWorkStops().size() + 1) continue;
					if (edge.from_index == run->getWorkStops().size() + 1) continue;
					if (edge.to_index == 0) continue;
					SIMPLE_LOG_TRACE(logger, "Trying to introduce suggested edge");
					if (edge_introducer.introduce(edge))
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
			suggestor->reset();
		}

		SIMPLE_LOG_DEBUG(logger, "Single run TSP finished");
	}

	void SuIntTSPSolver::setRoutingService(RoutingService* routing_service)
	{
		this->routing_service = routing_service;
	}

	void SuIntTSPSolver::setCostFunction(ScheduleCostFunction* cost_function)
	{
		this->cost_function = cost_function;
	}

	void SuIntTSPSolver::setEdgeSuggestor(const EdgeSuggestorType& type)
	{
		this->edge_suggestor_type = type;
	}

	void SuIntTSPSolver::addEdgeIntroducer(const EdgeIntroducerType& type)
	{
		this->edge_introducers_types.push_back(type);
	}
}
