#include "GCBIEdgeIntroducer.h"
#include <Engine/SceneEditor/SceneEditor.h>

namespace Scheduler
{
	GCBIEdgeIntroducer::GCBIEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor) :
		run(run),
		cost_function(cost_function),
		scene_editor(scene_editor),
		direct_edge_introducer(run, cost_function, scene_editor),
		circular_edge_introducer(run, cost_function, scene_editor),
		best_cost(cost_function->calculateCost(run->getSchedule()))
	{
	}

	bool GCBIEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		assert(edge.to_index != edge.from_index);
		assert(edge.from_index > 0 || edge.to_index < run->getWorkStops().size() + 1);
		assert(edge.from_index >= 0 && edge.to_index >= 0 && edge.from_index < run->getWorkStops().size() + 2 && edge.to_index < run->getWorkStops().size() + 2);

		if(edge.to_index - edge.from_index == 1) return false; // Nothing to improve - the edge is already present
		if(edge.from_index == 0 && edge.to_index == run->getWorkStops().size() + 2) return false; // There is no way to introduce start-end edge
		
		EdgeIntroducer &introducer = edge.from_index < edge.to_index ? static_cast<EdgeIntroducer&>(direct_edge_introducer) : static_cast<EdgeIntroducer&>(circular_edge_introducer);

		size_t checkpoint = scene_editor.checkpoint();
		if (introducer.introduce(edge))
		{
			Cost new_cost = cost_function->calculateCost(run->getSchedule());
			if (new_cost >= best_cost) {
				scene_editor.rollbackToCheckpoint(checkpoint);
				return false;
			}
			best_cost = new_cost;
			return true;
		}

		return false;
	}
}
