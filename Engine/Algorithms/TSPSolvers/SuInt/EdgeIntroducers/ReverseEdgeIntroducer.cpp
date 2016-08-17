#include "ReverseEdgeIntroducer.h"
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/ReverseRunWorkStopsSubsequence.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/MoveRunWorkStopsSubsequence.h>

namespace Scheduler
{
	ReverseEdgeIntroducer::ReverseEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor):
		run(run),
		run_iter(std::find(run->getSchedule()->getRuns().begin(), run->getSchedule()->getRuns().end(), run)),
		scene_editor(scene_editor)
	{
	}

	bool ReverseEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		if (edge.from_index >= edge.to_index || edge.to_index > run->getWorkStops().size()) {
			return false;
		}

		Iterator from_iter = std::next(run->getWorkStops().begin(), edge.from_index);
		Iterator to_iter = std::next(run->getWorkStops().begin(), edge.to_index);

		scene_editor.performAction<ReverseWorkStopsSubsequence>(run_iter, from_iter, to_iter);
		return true;
	}
}
