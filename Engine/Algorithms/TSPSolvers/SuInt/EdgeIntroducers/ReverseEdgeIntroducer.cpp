#include "ReverseEdgeIntroducer.h"
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/ReverseWorkStops.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneEditor/Actions/MoveWorkStops.h>

namespace Scheduler
{
	ReverseEdgeIntroducer::ReverseEdgeIntroducer(Run& run, SceneEditor& scene_editor):
		run(run),
		scene_editor(scene_editor)
	{
	}

	bool ReverseEdgeIntroducer::introduce(const SuggestedEdge& edge)
	{
		if (edge.from_index >= edge.to_index || edge.to_index > run.getWorkStops().size()) {
			return false;
		}

		Iterator from_iter = std::next(run.getWorkStops().begin(), edge.from_index);
		Iterator to_iter = std::next(run.getWorkStops().begin(), edge.to_index);

		scene_editor.performAction<ReverseWorkStops>(run, from_iter, to_iter);
		return true;
	}
}
