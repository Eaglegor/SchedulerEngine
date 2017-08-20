#include "CompositeEdgeIntroducer.h"
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkStop.h>
#include <algorithm>


namespace Scheduler
{
	CompositeEdgeIntroducer::CompositeEdgeIntroducer(Run& run, const ScheduleCostFunction& cost_function, SceneEditor& scene_editor)
	    : run(run),
	      cost_function(cost_function),
	      scene_editor(scene_editor),
	      best_cost(cost_function.calculateCost(run.getSchedule( ))),
	      logger(LoggingManager::getLogger("CompositeEdgeIntroducer"))
	{
	}

	bool CompositeEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		assert(edge.to_index != edge.from_index);
		assert(edge.from_index > 0 || edge.to_index < run.getWorkStops( ).size( ) + 1);
		assert(edge.from_index >= 0 && edge.to_index >= 0 && edge.from_index < run.getWorkStops( ).size( ) + 2 && edge.to_index < run.getWorkStops( ).size( ) + 2);

		if(edge.to_index - edge.from_index == 1)
		{
			LOG_TRACE(logger, "The edge is already present. Nothing to do.");
			return false;
		}

		if(edge.from_index == 0 && edge.to_index == run.getWorkStops( ).size( ) + 2)
		{
			LOG_TRACE(logger, "Start->end edge is not allowed.");
			return false;
		}

		std::size_t checkpoint = scene_editor.checkpoint( );
		for(auto& introducer : introducers)
		{
			if(introducer->introduce(edge))
			{
				Cost new_cost = cost_function.calculateCost(run.getSchedule( ));
				LOG_DEBUG(logger, "Cost after edge introduction: {} (best: {})", new_cost.getValue( ), best_cost.getValue( ));
				if(new_cost < best_cost)
				{
					LOG_TRACE(logger, "Improvement found");
					best_cost = new_cost;
					return true;
				}
				else
				{
					LOG_TRACE(logger, "No improvement found");
					scene_editor.rollbackToCheckpoint(checkpoint);
				}
			}
		}

		return false;
	}
}
