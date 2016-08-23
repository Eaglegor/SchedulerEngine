#pragma once
#include "../EdgeIntroducer.h"
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class CircularEdgeIntroducer : public EdgeIntroducer
	{
	public:
		CircularEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;
		
	private:
		using Iterator = Run::WorkStopsList::iterator;

		Run* run;
		ImmutableVector<Run*>::const_iterator run_iter;
		ScheduleCostFunction* cost_function;
		SceneEditor& scene_editor;
	};
}
