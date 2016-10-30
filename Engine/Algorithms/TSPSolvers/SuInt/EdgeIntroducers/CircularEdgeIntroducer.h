#pragma once
#include "../EdgeIntroducer.h"
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>
#include <Engine/SceneManager/Run.h>

namespace Scheduler
{
	class CircularEdgeIntroducer : public EdgeIntroducer
	{
	public:
		CircularEdgeIntroducer(Run& run, const ScheduleCostFunction& cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;
		
	private:
		using Iterator = Run::WorkStopsList::iterator;

		Run& run;
		const ScheduleCostFunction& cost_function;
		SceneEditor& scene_editor;
	};
}
