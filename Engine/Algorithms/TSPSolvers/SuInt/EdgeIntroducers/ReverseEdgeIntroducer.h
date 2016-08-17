#pragma once

#include "../EdgeIntroducer.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{
	class ReverseEdgeIntroducer : public EdgeIntroducer
	{
	public:
		ReverseEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

	private:
		using Iterator = Run::WorkStopsList::iterator;

		Run* run;
		ImmutableVector<Run*>::const_iterator run_iter;
		SceneEditor& scene_editor;
	};
}
