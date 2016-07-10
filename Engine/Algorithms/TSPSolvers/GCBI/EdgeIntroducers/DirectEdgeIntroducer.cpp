#include "DirectEdgeIntroducer.h"
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Schedule.h>

namespace Scheduler
{
	DirectEdgeIntroducer::DirectEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor):
		run(run),
		run_iter(std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run)),
		cost_function(cost_function),
		scene_editor(scene_editor)
	{
	}

	bool DirectEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		if (edge.from_index >= edge.to_index) {
			return false;
		}

		Iterator from_iter = run->getWorkStops().begin() + edge.from_index;
		Iterator to_iter = run->getWorkStops().begin() + (edge.to_index - 1);

		Iterator best_iterator;

		BestAllocationResult before_edge_allocation = getBestAllocationVariant(from_iter, to_iter, run->getWorkStops().begin(), from_iter);
		if (before_edge_allocation.iterator != run->getWorkStops().end()) best_iterator = before_edge_allocation.iterator;
		
		if (to_iter != run->getWorkStops().end())
		{
			BestAllocationResult after_edge_allocation = getBestAllocationVariant(from_iter, to_iter, to_iter + 1, run->getWorkStops().end());
			if (best_iterator == from_iter || after_edge_allocation.cost < before_edge_allocation.cost) best_iterator = after_edge_allocation.iterator;
		}

		if (best_iterator == from_iter) return false;

		scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, from_iter, to_iter, best_iterator);
		return true;
	}

	DirectEdgeIntroducer::BestAllocationResult DirectEdgeIntroducer::getBestAllocationVariant(Iterator subsequence_start, Iterator subsequence_end, Iterator target_range_start, Iterator target_range_end)
	{
		bool is_first = true;
		BestAllocationResult result;
		result.iterator = target_range_start;
		size_t checkpoint = scene_editor.checkpoint();
		for (auto target_iter = target_range_start; target_iter != target_range_end; ++target_iter)
		{
			scene_editor.performAction<MoveRunWorkStopsSubsequence>(run_iter, subsequence_start, subsequence_end, target_iter);
			Cost new_cost = cost_function->calculateCost(run->getSchedule());
			if (is_first || new_cost < result.cost)
			{
				is_first = false;
				result.cost = new_cost;
				result.iterator = target_iter;
			}
			scene_editor.rollbackToCheckpoint(checkpoint);
		}
		return result;
	}
}
