#pragma once

#include <Engine/AlgorithmsManager/CostFunction.h>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Scene;

	/**
	 * @todo Merge scene cost function and schedule cost function into the single class
	 * @todo Add supported factors list and the information about the cost function additivity
	 */
	class SceneCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Scene& scene) const = 0;
	};
}