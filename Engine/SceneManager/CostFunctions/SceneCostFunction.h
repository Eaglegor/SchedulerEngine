#pragma once

#include <Engine/Concepts/Cost.h>
#include <Engine/AlgorithmsManager/CostFunction.h>

namespace Scheduler
{
	class Scene;

    class SceneCostFunction : public CostFunction
    {
	public:
		virtual Cost calculateCost(const Scene& scene) const = 0;
	};
}