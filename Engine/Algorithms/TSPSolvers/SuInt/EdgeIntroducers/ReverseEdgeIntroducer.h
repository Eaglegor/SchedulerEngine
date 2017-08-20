#pragma once

#include "../EdgeIntroducer.h"
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class ReverseEdgeIntroducer : public EdgeIntroducer
	{
	public:
		ReverseEdgeIntroducer(Run& run, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

	private:
		using Iterator = Run::WorkStopsList::iterator;

		Run& run;
		SceneEditor& scene_editor;
	};
}
