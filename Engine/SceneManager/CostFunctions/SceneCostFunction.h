#pragma once

#include <Engine/AlgorithmsManager/CostFunction.h>
#include <Engine/Concepts/Cost.h>

namespace Scheduler
{
	class Scene;

	class SceneCostFunction : public CostFunction
	{
	public:
		virtual Cost calculateCost(const Scene& scene) const = 0;
	};
}