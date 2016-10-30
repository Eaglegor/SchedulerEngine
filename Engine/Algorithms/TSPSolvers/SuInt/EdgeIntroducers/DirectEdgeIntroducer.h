#pragma once

#include "../EdgeIntroducer.h"
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/CostFunctions/ScheduleCostFunction.h>

namespace Scheduler
{
	class DirectEdgeIntroducer : public EdgeIntroducer
	{
	public:
		DirectEdgeIntroducer(Run& run, const ScheduleCostFunction& cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

	private:
		using Iterator = Run::WorkStopIterator;

		struct BestAllocationResult
		{
			Cost cost;
			Iterator iterator;
		};

		BestAllocationResult getBestAllocationVariant(Iterator subsequence_start, Iterator subsequence_end, Iterator target_range_start, Iterator target_range_end);

		Run& run;
		const ScheduleCostFunction& cost_function;
		SceneEditor& scene_editor;
	};
}
