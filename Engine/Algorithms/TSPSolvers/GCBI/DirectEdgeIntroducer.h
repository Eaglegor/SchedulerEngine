#pragma once

#include "EdgeIntroducer.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/StrategiesManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{
	class DirectEdgeIntroducer : public EdgeIntroducer
	{
	public:
		DirectEdgeIntroducer(Run* run, ScheduleCostFunction* cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

	private:
		using Iterator = ImmutableVector<WorkStop*>::const_iterator;

		struct BestAllocationResult
		{
			Cost cost;
			Iterator iterator;
		};

		BestAllocationResult getBestAllocationVariant(Iterator subsequence_start, Iterator subsequece_end, Iterator target_range_start, Iterator target_range_end);

		Run* run;
		ImmutableVector<Run*>::const_iterator run_iter;
		ScheduleCostFunction* cost_function;
		SceneEditor& scene_editor;
	};
}
