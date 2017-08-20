#include "SuIntTSPSolver.h"
#include "BetterEdgeSuggestor.h"
#include "DistanceRatingEdgeSuggestor.h"
#include "EdgeIntroducer.h"
#include "EdgeIntroducers/CircularEdgeIntroducer.h"
#include "EdgeIntroducers/CompositeEdgeIntroducer.h"
#include "EdgeIntroducers/DirectEdgeIntroducer.h"
#include "EdgeIntroducers/ReverseEdgeIntroducer.h"
#include "EdgeSuggestor.h"
#include "SuggestedEdge.h"
#include <Engine/Concepts/Route.h>
#include <Engine/SceneEditor/Actions/MoveWorkStops.h>
#include <Engine/SceneEditor/Actions/RotateWorkStops.h>
#include <Engine/SceneEditor/Actions/SwapWorkStops.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/WorkStop.h>
#include <algorithm>
#include <queue>

namespace Scheduler
{
	SuIntTSPSolver::SuIntTSPSolver( )
	    : logger(LoggingManager::getLogger("SuIntTSPSolver")),
	      edge_suggestor_type(EdgeSuggestorType::BETTER_EDGE)
	{
	}

	void SuIntTSPSolver::optimize(Schedule& schedule) const
	{
		TRACEABLE_SECTION(__optimize__, "SuIntTSPSolver::optimize(Schedule&)", logger);

		if(!routing_service || !cost_function)
		{
			if(!routing_service) LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if(!cost_function) LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		for(Run& r : schedule.getRuns( ))
		{
			optimize(r);
		}
	}

	void SuIntTSPSolver::optimize(Run& run) const
	{
		TRACEABLE_SECTION(__optimize__, "SuIntTSPSolver::optimize(Run&)", logger);

		if(!routing_service || !cost_function)
		{
			if(!routing_service) LOG_WARNING(logger, "Routing service is not set. Can't solve TSP.");
			if(!cost_function) LOG_WARNING(logger, "Cost function is not set. Can't solve TSP.");
			return;
		}

		LOG_TRACE(logger, "Initializing");

		SceneEditor scene_editor;

		std::unique_ptr<EdgeSuggestor> suggestor;

		switch(edge_suggestor_type)
		{
			case EdgeSuggestorType::BETTER_EDGE:
			{
				suggestor.reset(new BetterEdgeSuggestor(run, routing_service.get( )));
				break;
			}
			case EdgeSuggestorType::DISTANCE_RATING:
			{
				suggestor.reset(new DistanceRatingEdgeSuggestor(run, routing_service.get( )));
				break;
			}
		}

		CompositeEdgeIntroducer edge_introducer(run, cost_function.get( ), scene_editor);
		for(const EdgeIntroducerType& introducer_type : edge_introducers_types)
		{
			switch(introducer_type)
			{
				case EdgeIntroducerType::CIRCULAR:
				{
					edge_introducer.addIntroducer<CircularEdgeIntroducer>(run, cost_function.get( ), scene_editor);
					break;
				}
				case EdgeIntroducerType::DIRECT:
				{
					edge_introducer.addIntroducer<DirectEdgeIntroducer>(run, cost_function.get( ), scene_editor);
					break;
				}
				case EdgeIntroducerType::REVERSE:
				{
					edge_introducer.addIntroducer<ReverseEdgeIntroducer>(run, scene_editor);
					break;
				}
			}
		}

		assert(suggestor);

		{
			TRACEABLE_SECTION(__main_loop__, "SuIntTSPSolver::optimize: main loop", logger);

			bool finished = false;

			while(!finished)
			{
				TRACEABLE_SECTION(__outer_iteration__, "SuIntTSPSolver::optimize: outer iteration", logger);

				finished = true;
				while(suggestor->hasNext( ))
				{
					std::vector<SuggestedEdge> edges = suggestor->next( );
					for(SuggestedEdge edge : edges)
					{
						LOG_DEBUG(logger, "Suggested edge: {}-{}", edge.from_index, edge.to_index);
						if(edge.from_index == edge.to_index) continue;
						if(edge.from_index == 0 && edge.to_index == run.getWorkStops( ).size( ) + 1) continue;
						if(edge.from_index == run.getWorkStops( ).size( ) + 1) continue;
						if(edge.to_index == 0) continue;
						LOG_TRACE(logger, "Trying to introduce suggested edge");
						if(edge_introducer.introduce(edge))
						{
							LOG_TRACE(logger, "Introduction successful. Commiting changes");
							scene_editor.commit( );
							finished = false;
							break;
						}
						else
						{
							LOG_TRACE(logger, "Introduction failed. Rolling changes back");
							scene_editor.rollbackAll( );
						}
					}
				}
				suggestor->reset( );
			}
		}
	}

	void SuIntTSPSolver::setRoutingService(const RoutingService& routing_service)
	{
		this->routing_service = routing_service;
	}

	void SuIntTSPSolver::setCostFunction(const ScheduleCostFunction& cost_function)
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
