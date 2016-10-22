#include "CircularEdgeIntroducer.h"

#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/RotateWorkStops.h>

namespace Scheduler
{
	CircularEdgeIntroducer::CircularEdgeIntroducer(Run& run, const ScheduleCostFunction& cost_function, SceneEditor& scene_editor):
		run(run),
		cost_function(cost_function),
		scene_editor(scene_editor)
	{
	}

	bool CircularEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		if(edge.from_index <= edge.to_index)
		{
			return false;
		}

		if(edge.to_index == 0)
		{
			return false;
		}

		Iterator subsequence_start = std::next(run.getWorkStops().begin(), edge.to_index - 1);
		Iterator subsequence_end = std::next(run.getWorkStops().begin(), edge.from_index);

	 	bool first = true;
		Cost best_cost;
		Iterator best_iterator = run.getWorkStops().end();

		std::size_t checkpoint = scene_editor.checkpoint();
		for (auto new_first_iter = std::next(subsequence_start); new_first_iter != subsequence_end; ++new_first_iter)
		{
			scene_editor.performAction<RotateWorkStops>(run, subsequence_start, new_first_iter, subsequence_end);
			Cost new_cost = cost_function.calculateCost(run.getSchedule());
			if(first || new_cost < best_cost)
			{
				first = false;
				best_cost = new_cost;
				best_iterator = new_first_iter;
			}
			scene_editor.rollbackToCheckpoint(checkpoint);
		}

		if (best_iterator == run.getWorkStops().end()) return false;

		scene_editor.performAction<RotateWorkStops>(run, subsequence_start, best_iterator, subsequence_end);
		return true;
	}
}
