#pragma once

#include "EdgeIntroducer.h"
#include "DirectEdgeIntroducer.h"
#include "CircularEdgeIntroducer.h"

namespace Scheduler
{
	class ScheduleCostFunction;
	class Run;

	class GCBIEdgeIntroducer : public EdgeIntroducer
	{
	public:
		GCBIEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

	private:
		Run* run;
		ScheduleCostFunction* cost_function;
		SceneEditor& scene_editor;
		Cost best_cost;

		DirectEdgeIntroducer direct_edge_introducer;
		CircularEdgeIntroducer circular_edge_introducer;
	};
}