#pragma once

#include "../EdgeIntroducer.h"
#include <Engine/Concepts/Cost.h>
#include <Engine/LoggingManager/LoggingManager.h>
#include <memory>
#include <vector>

namespace Scheduler
{
	class ScheduleCostFunction;
	class Run;

	class CompositeEdgeIntroducer : public EdgeIntroducer
	{
	public:
		CompositeEdgeIntroducer(Run& run, const ScheduleCostFunction& cost_function, SceneEditor& scene_editor);

		virtual bool introduce(const SuggestedEdge& edge) override;

		template <typename IntroducerType, typename... Args>
		void addIntroducer(Args&&... args)
		{
			std::shared_ptr<EdgeIntroducer> introducer = std::make_shared<IntroducerType>(std::forward<Args>(args)...);
			introducers.push_back(introducer);
		}

	private:
		Run& run;
		const ScheduleCostFunction& cost_function;
		SceneEditor& scene_editor;
		Cost best_cost;

		std::vector<std::shared_ptr<EdgeIntroducer>> introducers;

		Logger& logger;
	};
}